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

#ifndef DEMO_COMMON_H
#define DEMO_COMMON_H

#include <string>
#include <vector>
#include "util_draw.h"
#include "util_input.h"

void get_jpeg_image_names(const std::string &input_image_path, std::vector<std::string> &image_names);
void print_demo_title(dmp::util::COverlayRGB &bg_overlay, const char *titles[2]);
void print_conv_time(dmp::util::COverlayRGB &bg_overlay, int y, int conv_time, const std::string &conv_freq);
void print_fc_time(dmp::util::COverlayRGB &bg_overlay, int y, int fc_time, const std::string &fc_freq);
void print_total_time(dmp::util::COverlayRGB &bg_overlay, int y, int total_time);
void handle_keyboard_input(int &exit_code, bool &pause);

#endif
