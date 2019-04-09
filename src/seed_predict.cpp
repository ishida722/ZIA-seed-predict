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

#include "pumpkin20181218_verylight_gen.h"
#include "util_draw.h"
#include "util_input.h"
#include "demo_common.h"
#include "imagenet_1000_categories.h"
#include "opencv2/opencv.hpp"

using namespace std;
using namespace dmp;
using namespace util;

#define FILENAME_WEIGHTS "pumpkin20181218_verylight_weights.bin"

#define SCREEN_W (get_screen_width())
#define SCREEN_H (get_screen_height())

#define IMAGE_W 640
#define IMAGE_H 512

#define CIMAGE_W 320
#define CIMAGE_H 240

#define PIMAGE_W 64
#define PIMAGE_H 64

#define ERR(...) fprintf(stderr, __VA_ARGS__); fflush(stderr)

const uint32_t RED = 0x00FF0000;
const uint32_t BLUE = 0x000000FF;

// Define CNN network model object
/* CKerasGoogLeNet network; */
Cpumpkin20181218_verylight network; 

// Categories strings
std::vector<std::string> catstr_vec(categories, categories + 1000);
std::vector<std::string> seed_strings = { "NG","OK" };
std::vector<uint32_t> seed_colors = {RED, BLUE};


void preproc_image_gray(uint32_t *__restrict imgView, __fp16 *__restrict imgProc,
                   int img_width, int img_height, float r_offs, float g_offs,
                   float b_offs, float sf, bool transpose, bool is_bgr) {
  int i = 0, j = 0;
  for (int y = 0; y < img_height; y++) {
    if (transpose) j = 1 * y;
    for (int x = 0; x < img_width; x++) {
      uint32_t rgba = imgView[i];
      i++;
      unsigned char r = (rgba >> 24);
      unsigned char g = (rgba >> 16) & 0xFF;
      unsigned char b = (rgba >> 8) & 0xFF;
      imgProc[j] = (__fp16)(sf * (b_offs + (r+g+b)/3));
      j++;
      }
      if (transpose) j += 1 * (img_height - 1);
    }
  }


void preproc_image_gray(uint32_t *__restrict imgView, __fp16 *__restrict imgProc,
                   int inimg_width, int inimg_height, int outimg_width,
                   int outimg_height, float r_offs, float g_offs, float b_offs,
                   float sf, bool transpose, bool is_bgr) {
  uint32_t *imgIn = imgView, *imgOut = nullptr;

  while (inimg_width >= outimg_width * 2 && inimg_height >= outimg_height * 2) {
    int nw = inimg_width / 2;
    int nh = inimg_height / 2;
    imgOut = new uint32_t[nw * nh];
    for (int y = 0; y < nh; y++) {
      for (int x = 0; x < nw; x++) {
        uint32_t p, pr = 0, pg = 0, pb = 0;
        p = imgIn[(y * 2 + 0) * inimg_width + (x * 2 + 0)];
        pr += (p >> 24);
        pg += (p >> 16) & 0xFF;
        pb += (p >> 8) & 0xFF;
        p = imgIn[(y * 2 + 0) * inimg_width + (x * 2 + 1)];
        pr += (p >> 24);
        pg += (p >> 16) & 0xFF;
        pb += (p >> 8) & 0xFF;
        p = imgIn[(y * 2 + 1) * inimg_width + (x * 2 + 0)];
        pr += (p >> 24);
        pg += (p >> 16) & 0xFF;
        pb += (p >> 8) & 0xFF;
        p = imgIn[(y * 2 + 1) * inimg_width + (x * 2 + 1)];
        pr += (p >> 24);
        pg += (p >> 16) & 0xFF;
        pb += (p >> 8) & 0xFF;
        pr /= 4;
        pg /= 4;
        pb /= 4;
        imgOut[y * nw + x] = (pr << 24) | (pg << 16) | (pb << 8);
      }
    }
    inimg_width = nw;
    inimg_height = nh;
    if (imgIn != imgView) delete[] imgIn;
    imgIn = imgOut;
  }

  while (inimg_width >= outimg_width * 2) {
    int nw = inimg_width / 2;
    int nh = inimg_height;
    imgOut = new uint32_t[nw * nh];
    for (int y = 0; y < nh; y++) {
      for (int x = 0; x < nw; x++) {
        uint32_t p, pr = 0, pg = 0, pb = 0;
        p = imgIn[y * inimg_width + (x * 2 + 0)];
        pr += (p >> 24);
        pg += (p >> 16) & 0xFF;
        pb += (p >> 8) & 0xFF;
        p = imgIn[y * inimg_width + (x * 2 + 1)];
        pr += (p >> 24);
        pg += (p >> 16) & 0xFF;
        pb += (p >> 8) & 0xFF;
        pr /= 2;
        pg /= 2;
        pb /= 2;
        imgOut[y * nw + x] = (pr << 24) | (pg << 16) | (pb << 8);
      }
    }
    inimg_width = nw;
    if (imgIn != imgView) delete[] imgIn;
    imgIn = imgOut;
  }

  while (inimg_height >= outimg_height * 2) {
    int nw = inimg_width;
    int nh = inimg_height / 2;
    imgOut = new uint32_t[nw * nh];
    for (int y = 0; y < nh; y++) {
      for (int x = 0; x < nw; x++) {
        uint32_t p, pr = 0, pg = 0, pb = 0;
        p = imgIn[(y * 2 + 0) * inimg_width + x];
        pr += (p >> 24);
        pg += (p >> 16) & 0xFF;
        pb += (p >> 8) & 0xFF;
        p = imgIn[(y * 2 + 1) * inimg_width + x];
        pr += (p >> 24);
        pg += (p >> 16) & 0xFF;
        pb += (p >> 8) & 0xFF;
        pr /= 2;
        pg /= 2;
        pb /= 2;
        imgOut[y * nw + x] = (pr << 24) | (pg << 16) | (pb << 8);
      }
    }
    inimg_height = nh;
    if (imgIn != imgView) delete[] imgIn;
    imgIn = imgOut;
  }

  if (inimg_width != outimg_width || inimg_height != outimg_height) {
    int nw = outimg_width;
    int nh = outimg_height;
    float sx = float(inimg_width) / float(outimg_width);
    float sy = float(inimg_height) / float(outimg_height);
    imgOut = new uint32_t[nw * nh];
    for (int y = 0; y < nh; y++) {
      float ty = y * sy;
      int iy = int(ty);
      float ry = ty - iy;
      for (int x = 0; x < nw; x++) {
        float tx = x * sx;
        int ix = int(tx);
        float rx = tx - ix;
        float r = 0.f, g = 0.f, b = 0.f;
        uint32_t p;
        p = imgIn[(iy + 0) * inimg_width + (ix + 0)];
        r += (p >> 24) * rx * ry;
        g += ((p >> 16) & 0xFF) * rx * ry;
        b += ((p >> 8) & 0xFF) * rx * ry;
        p = imgIn[(iy + 0) * inimg_width + (ix + 1)];
        r += (p >> 24) * (1.f - rx) * ry;
        g += ((p >> 16) & 0xFF) * (1.f - rx) * ry;
        b += ((p >> 8) & 0xFF) * (1.f - rx) * ry;
        p = imgIn[(iy + 1) * inimg_width + (ix + 0)];
        r += (p >> 24) * rx * (1.f - ry);
        g += ((p >> 16) & 0xFF) * rx * (1.f - ry);
        b += ((p >> 8) & 0xFF) * rx * (1.f - ry);
        p = imgIn[(iy + 1) * inimg_width + (ix + 1)];
        r += (p >> 24) * (1.f - rx) * (1.f - ry);
        g += ((p >> 16) & 0xFF) * (1.f - rx) * (1.f - ry);
        b += ((p >> 8) & 0xFF) * (1.f - rx) * (1.f - ry);
        unsigned char pr = static_cast<unsigned char>(r);
        unsigned char pg = static_cast<unsigned char>(g);
        unsigned char pb = static_cast<unsigned char>(b);
        imgOut[y * nw + x] = (pr << 24) | (pg << 16) | (pb << 8);
      }
    }
    inimg_width = nw;
    inimg_height = nh;
    if (imgIn != imgView) delete[] imgIn;
    imgIn = imgOut;
  }
  preproc_image_gray(imgIn, imgProc, outimg_width, outimg_height, r_offs, g_offs,
                b_offs, sf, transpose, is_bgr);
  if (imgIn != imgView) delete[] imgIn;
}
int main(int argc, char **argv) {
    cout << "start" << endl;

    // Initialize frame buffer
    if (!init_fb()) {
        return 1;
    }

    // Initialize WebCam
    if (dmp::util::open_cam(CIMAGE_W, CIMAGE_H, 20)) {
        cout << "Camera init error" << endl;
        return -1;
    }

    /* Initialize network object */
        network.Verbose(0);
    if (!network.Initialize()) {
        ERR("Failed network init\n");
        return -1;
    }
    if (!network.LoadWeights(FILENAME_WEIGHTS)) {
        ERR("Failed load weights\n");
        return -1;
    }
    if (!network.Commit()) {
        ERR("Failed network commit\n");
        return -1;
    }

    cout << "Netowork init ok" << endl;

    // Get HW module frequency
    string conv_freq;
    conv_freq = std::to_string(network.get_dv_info().conv_freq);

    // Buffer for decoded image data
    uint32_t imgView[IMAGE_W * IMAGE_H];
    // Buffer for pre-processed image data
    __fp16 imgProc[PIMAGE_W * PIMAGE_H * 1];
    // buffer for result


    COverlayRGB bg_overlay(SCREEN_W, SCREEN_H);
    bg_overlay.alloc_mem_overlay(SCREEN_W, SCREEN_H);
    bg_overlay.load_ppm_img("fpgatitle");
    COverlayRGB cam_overlay(SCREEN_W, SCREEN_H);
    cam_overlay.alloc_mem_overlay(CIMAGE_W, CIMAGE_H);


    // Draw background two times for front and back buffer
    const char *titles[] = {
        "Seed Predect",
    };
    for (int i = 0; i < 2; ++i) {
        bg_overlay.print_to_display(0, 0);
        print_demo_title(bg_overlay, titles);
        swap_buffer();
    }
    cout << "Draw BG" << endl;

    int exit_code = -1;
    bool pause = false;
    std::vector<float> tensor;
    std::vector<float> boxes;
    cv::Mat frame;

    cout << "Start Main Loop" << endl;
    // Enter main loop
    while (exit_code == -1) {
        // If not pause, get next image from WebCam
        if (!pause) {
            if (capture_cam(imgView, CIMAGE_W, CIMAGE_H, 0, 0, CIMAGE_W, CIMAGE_H))
            {
                cout << "Camera error" << endl;
                break;
            }
            // 推測にまわすデータを作成
            cam_overlay.convert_to_overlay_pixel_format(imgView, CIMAGE_W*CIMAGE_H);
            // Pre-process the image data
            preproc_image_gray(imgView, imgProc, IMAGE_W, IMAGE_H, PIMAGE_W, PIMAGE_H, 0.0, 0.0, 0.0, 1.0 / 255.0, true, false);
        }

        // Run network in HW
        memcpy(network.get_network_input_addr_cpu(), imgProc, PIMAGE_W * PIMAGE_H * 2);
        network.RunNetwork();

        // Handle output from HW
        network.get_final_output(tensor);
        std::vector<std::pair<float, int>> ranks;
        ranks = catrank(&tensor.front(), (int)tensor.size());
        int label_num = ranks[0].second;
        std::string label = seed_strings[label_num];
        /* float score = ranks[0].first; */

        // Result
        unsigned text_size = 20;
        unsigned w = 0;
        unsigned h = 0;
        string s = label;
        COverlayRGB::calculate_boundary_text(s, text_size, w, h);
        COverlayRGB predict_text(SCREEN_W, SCREEN_H);
        predict_text.alloc_mem_overlay(w, h);

        int x = SCREEN_W / 2;
        int y = SCREEN_H * 0.75;

        //clear previous text
        COverlayRGB clear_bg(SCREEN_W, SCREEN_H);
        clear_bg.alloc_mem_overlay(SCREEN_W, h);
        clear_bg.copy_overlay(bg_overlay, 0, y);
        clear_bg.print_to_display(0, y);

        predict_text.copy_overlay(bg_overlay,x, y);
        predict_text.set_text(0, 0, s, text_size, seed_colors[label_num]);
        predict_text.print_to_display(x, y);

        cam_overlay.print_to_display(((SCREEN_W - CIMAGE_W) / 2), 145);
        swap_buffer();
        handle_keyboard_input(exit_code, pause);
    }

    shutdown();
    close_cam();

    return exit_code;
}
