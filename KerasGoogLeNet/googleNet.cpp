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

#include "KerasGoogLeNet_gen.h"
#include "util_draw.h"
#include "util_input.h"
#include "demo_common.h"
#include "imagenet_1000_categories.h"

using namespace std;
using namespace dmp;
using namespace util;

#define SCREEN_W (get_screen_width())
#define SCREEN_H (get_screen_height())

#define IMAGE_W 224
#define IMAGE_H 224

#define FILENAME_WEIGHTS "KerasGoogLeNet_weights.bin"

// Define CNN network model object
CKerasGoogLeNet network;

// Categories strings
std::vector<std::string> catstr_vec(categories, categories + 1000);

// Buffer for decoded image data
uint32_t imgView[IMAGE_W * IMAGE_H];
// Buffer for pre-processed image data
__fp16 imgProc[IMAGE_W * IMAGE_H * 3];

int main(int argc, char** argv) {
  // Initialize FB
  if (!init_fb()) {
    cout << "init_fb() failed." << endl;
    return 1;
  }

  // Get input images filenames
  vector<string> image_names;
  get_jpeg_image_names("./images/", image_names);
  int num_images = image_names.size();
  if (num_images == 0) {
    cout << "No input images." << endl;
    return 1;
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
  string conv_freq, fc_freq;
  conv_freq = std::to_string(network.get_dv_info().conv_freq);
  fc_freq = std::to_string(network.get_dv_info().fc_freq);

  // Create background and image overlay
  COverlayRGB bg_overlay(SCREEN_W, SCREEN_H);
  bg_overlay.alloc_mem_overlay(SCREEN_W, SCREEN_H);
  bg_overlay.load_ppm_img("fpgatitle");
  COverlayRGB overlay_input(SCREEN_W, SCREEN_H);
  overlay_input.alloc_mem_overlay(IMAGE_W, IMAGE_H);

  // Draw background two times for front and back buffer
  const char *titles[] = {
    "CNN - GoogLeNet",
    "Object Class Identification",
  };
  for (int i = 0; i < 2; ++i) {
    bg_overlay.print_to_display(0, 0);
    print_demo_title(bg_overlay, titles);
    swap_buffer();
  }

  int exit_code = -1;
  int image_nr = 0;
  bool pause = false;
  std::vector<float> network_output;
  // Enter main loop
  while (exit_code == -1) {
    // If not pause, decode next JPEG image and do pre-processing
    if (!pause) {
      decode_jpg_file(image_names[image_nr], imgView, IMAGE_W, IMAGE_H);
      overlay_input.convert_to_overlay_pixel_format(imgView, IMAGE_W * IMAGE_H);
      preproc_image(imgView, imgProc, IMAGE_W, IMAGE_H, -128.0, -128.0, -128.0,
                    1.0, true);
      ++image_nr;
      image_nr %= num_images;
    }

    // Run network in HW
    memcpy(network.get_network_input_addr_cpu(), imgProc, IMAGE_W * IMAGE_H * 6);
    network.RunNetwork();

    // Handle output from HW
    network.get_final_output(network_output);
    overlay_input.print_to_display((SCREEN_W - IMAGE_W) / 2, 185);

    // Print identification result to screen
    // print_result and catrank functions are defined in util_draw.cpp
    print_result(catstr_vec, (SCREEN_W / 5), IMAGE_W + 245,
                 catrank(&network_output.front()), bg_overlay);

    // Output HW processing times
    int conv_time_tot = network.get_conv_usec();
    print_conv_time(bg_overlay, 7 * SCREEN_H / 8, conv_time_tot, conv_freq);
    int fc_time_tot = network.get_fc_usec();
    print_fc_time(bg_overlay, 7 * SCREEN_H / 8 + 28, fc_time_tot, fc_freq);
    print_total_time(bg_overlay, 7 * SCREEN_H / 8 + 56, conv_time_tot + fc_time_tot);

    swap_buffer();

    handle_keyboard_input(exit_code, pause);
  }

  shutdown();

  return exit_code;
}
