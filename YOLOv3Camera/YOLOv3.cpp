/*
 *  Copyright 2018 Digital Media Professionals Inc.
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <algorithm>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>
#include <sstream>
#include <string>
#include <vector>

#include "YOLOv3_gen.h"
#include "util_draw.h"
#include "util_input.h"
#include "demo_common.h"

using namespace std;
using namespace dmp;
using namespace util;

#define SCREEN_W (get_screen_width())
#define SCREEN_H (get_screen_height())

#ifdef __aarch64__
#define IMAGE_W 640
#define IMAGE_H 512

#define CIMAGE_W 640
#define CIMAGE_H 480
#else
#define IMAGE_W 320
#define IMAGE_H 256

#define CIMAGE_W 320
#define CIMAGE_H 240
#endif

#define PIMAGE_W 320
#define PIMAGE_H 256

#define FILENAME_WEIGHTS "YOLOv3_weights.bin"

// Define CNN network model object
CYOLOv3 network;

// Buffer for decoded image data
uint32_t imgView[IMAGE_W * IMAGE_H];
// Buffer for pre-processed image data
__fp16 imgProc[PIMAGE_W * PIMAGE_H * 3];

// Post-processing functions, defined in YOLOv3_post.cpp
void get_bboxes(const vector<float> &tensor, vector<float> &boxes);
void draw_bboxes(const vector<float> &boxes, COverlayRGB &overlay);

int main(int argc, char **argv) {
  // Initialize FB
  if (!init_fb()) {
    cout << "init_fb() failed." << endl;
    return 1;
  }

  // Initialize WebCam
  if (dmp::util::open_cam(CIMAGE_W, CIMAGE_H, 20)) {
    return -1;
  }

  // Initialize network object
  network.Verbose(0);
  if (!network.Initialize()) {
    return -1;
  }
  if (!network.LoadWeights(FILENAME_WEIGHTS)) {
    return -1;
  }
  if (!network.Commit()) {
    return -1;
  }

  // Get HW module frequency
  string conv_freq;
  conv_freq = std::to_string(network.get_dv_info().conv_freq);

  // Create background and image overlay
  COverlayRGB bg_overlay(SCREEN_W, SCREEN_H);
  bg_overlay.alloc_mem_overlay(SCREEN_W, SCREEN_H);
  bg_overlay.load_ppm_img("fpgatitle");
  COverlayRGB cam_overlay(SCREEN_W, SCREEN_H);
  cam_overlay.alloc_mem_overlay(CIMAGE_W, CIMAGE_H);

  // Draw background two times for front and back buffer
  const char *titles[] = {
    "CNN - Object Detection",
    "Bounding Box and Object Class detection",
  };
  for (int i = 0; i < 2; ++i) {
    bg_overlay.print_to_display(0, 0);
    print_demo_title(bg_overlay, titles);
    swap_buffer();
  }

  int exit_code = -1;
  bool pause = false;
  std::vector<float> tensor;
  std::vector<float> boxes;
  // Enter main loop
  while (exit_code == -1) {
    // If not pause, get next image from WebCam
    if (!pause) {
      if (capture_cam(imgView, CIMAGE_W, CIMAGE_H, 0, 0, CIMAGE_W, CIMAGE_H)) {
        break;
      }
      cam_overlay.convert_to_overlay_pixel_format(imgView, CIMAGE_W*CIMAGE_H);
      // Pre-process the image data
      preproc_image(imgView, imgProc, IMAGE_W, IMAGE_H, PIMAGE_W, PIMAGE_H,
                    0.0, 0.0, 0.0, 1.0 / 255.0, true, false);
    }

    // Run network in HW
    memcpy(network.get_network_input_addr_cpu(), imgProc, PIMAGE_W * PIMAGE_H * 6);
    network.RunNetwork();

    // Handle output from HW
    network.get_final_output(tensor);
    get_bboxes(tensor, boxes);
    draw_bboxes(boxes, cam_overlay);

    // Draw detection result to screen
    cam_overlay.print_to_display(((SCREEN_W - CIMAGE_W) / 2), 145);

    // Output HW processing times
    int conv_time_tot = network.get_conv_usec();
    print_conv_time(bg_overlay, (165 + CIMAGE_H), conv_time_tot, conv_freq);

    swap_buffer();

    handle_keyboard_input(exit_code, pause);
  }

  shutdown();
  close_cam();

  return exit_code;
}
