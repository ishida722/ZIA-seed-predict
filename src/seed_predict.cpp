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

// Define CNN network model object
/* CKerasGoogLeNet network; */
Cpumpkin20181218_verylight network; 

// Categories strings
std::vector<std::string> catstr_vec(categories, categories + 1000);
std::vector<std::string> seed_strings = { "NG","OK" };


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
    __fp16 imgProc[PIMAGE_W * PIMAGE_H * 3];

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
            /* cap.read(frame); */
            if (capture_cam(imgView, CIMAGE_W, CIMAGE_H, 0, 0, CIMAGE_W, CIMAGE_H))
            {
                cout << "Camera error" << endl;
                break;
            }
            // 推測にまわすデータを作成
            cam_overlay.convert_to_overlay_pixel_format(imgView, CIMAGE_W*CIMAGE_H);
            // Pre-process the image data
            preproc_image(imgView, imgProc, IMAGE_W, IMAGE_H, PIMAGE_W, PIMAGE_H, 0.0, 0.0, 0.0, 1.0 / 255.0, true, false);
        }

        // Run network in HW
        memcpy(network.get_network_input_addr_cpu(), imgProc, PIMAGE_W * PIMAGE_H * 6);
        network.RunNetwork();

        // Handle output from HW
        network.get_final_output(tensor);
        std::vector<std::pair<float, int>> ranks;
        ranks = catrank(&tensor.front(), (int)tensor.size());
        int label_num = ranks[0].second;
        std::string label = seed_strings[label_num];
        /* float score = ranks[0].first; */
        cout << label << endl;

        /* UpdateFrameBuffer(tensor, conv_freq); */
        bg_overlay.set_text(0, 0, label, 10, 0, 10);
        bg_overlay.print_to_display(0, 0);
        cam_overlay.print_to_display(((SCREEN_W - CIMAGE_W) / 2), 145);
        swap_buffer();
        handle_keyboard_input(exit_code, pause);
    }

    shutdown();
    close_cam();

    return exit_code;
}
