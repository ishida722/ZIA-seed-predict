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

#include "demo_common.h"

using namespace std;
using namespace dmp;
using namespace util;

void get_jpeg_image_names(const string &input_image_path, vector<string> &image_names) {
  const std::vector<std::string> input_image_suffix = {".jpg", ".jpeg", ".JPG",
                                                       ".JPEG"};

  image_names = dmp::util::get_input_image_names(input_image_path, input_image_suffix);
}

void print_demo_title(COverlayRGB &bg_overlay, const char *titles[2]) {
  unsigned text_size = 30;
  string font_file = "font/NotoSerif-Black.ttf";
  string text = titles[0];
  unsigned w = 0;
  unsigned h = 0;
  
  COverlayRGB::calculate_boundary_text_with_font(font_file, text, text_size, w, h);
  int x = ((get_screen_width() - w) / 2);
  int y = 25;
  COverlayRGB bg_text(get_screen_width(), get_screen_height());
  bg_text.alloc_mem_overlay(w, h);
  bg_text.copy_overlay(bg_overlay, x, y);
  bg_text.set_text_with_font(font_file, text, 0, 3*h/4, text_size, 0x00ffffff);
  bg_text.print_to_display(x, y);

  text = titles[1];
  COverlayRGB::calculate_boundary_text_with_font(font_file, text, text_size, w, h);
  x = ((get_screen_width() - w) / 2);
  y = 65;
  bg_text.delete_overlay();
  bg_text.alloc_mem_overlay(w, h);
  bg_text.copy_overlay(bg_overlay, x, y);
  bg_text.set_text_with_font(font_file, text, 0, 3*h/4, text_size, 0x00ffffff);
  bg_text.print_to_display(x, y);

  text_size = 11;
  text = "Copyright 2018. Digital Media Professionals Inc.";
  COverlayRGB::calculate_boundary_text_with_font(font_file, text, text_size, w, h);
  x = 5;
  y = get_screen_height() - 20;
  bg_text.delete_overlay();
  bg_text.alloc_mem_overlay(w, h);
  bg_text.copy_overlay(bg_overlay, x, y);
  bg_text.set_text_with_font(font_file, text, 0, 3*h/4, text_size, 0x00ffffff);
  bg_text.print_to_display(x, y);
}

void print_conv_time(dmp::util::COverlayRGB &bg_overlay, int y,
                     int conv_time, const std::string &conv_freq) {
  string text = COverlayRGB::convert_time_to_text("Convolution (" + conv_freq + " MHz HW ACC)     : ", conv_time);
  unsigned text_size = 14;

  unsigned w = 0;
  unsigned h = 0;
  COverlayRGB::calculate_boundary_text(text, text_size, w, h);
  int x = ((get_screen_width() - w) / 2);

  COverlayRGB overlay_time(get_screen_width(), get_screen_height());
  overlay_time.alloc_mem_overlay(w, h);
  overlay_time.copy_overlay(bg_overlay, x, y);
  overlay_time.set_text(0, 0, text, text_size, 0x00f4419d);
  overlay_time.print_to_display(x, y);
}

void print_fc_time(dmp::util::COverlayRGB &bg_overlay, int y,
                   int fc_time, const std::string &fc_freq) {
  string text = COverlayRGB::convert_time_to_text("Fully Connected (" + fc_freq + " MHz HW ACC) : ", fc_time);
  unsigned text_size = 14;

  unsigned w = 0;
  unsigned h = 0;
  COverlayRGB::calculate_boundary_text(text, text_size, w, h);
  int x = ((get_screen_width() - w) / 2);

  COverlayRGB overlay_time(get_screen_width(), get_screen_height());
  overlay_time.alloc_mem_overlay(w, h);
  overlay_time.copy_overlay(bg_overlay, x, y);
  overlay_time.set_text(0, 0, text, text_size, 0x00f4419d);
  overlay_time.print_to_display(x, y);
}

void print_total_time(dmp::util::COverlayRGB &bg_overlay, int y, int total_time) {
  string text = COverlayRGB::convert_time_to_text("Total Processing Time           : ", total_time);
  unsigned text_size = 14;

  unsigned w = 0;
  unsigned h = 0;
  COverlayRGB::calculate_boundary_text(text, text_size, w, h);
  int x = ((get_screen_width() - w) / 2);

  COverlayRGB overlay_time(get_screen_width(), get_screen_height());
  overlay_time.alloc_mem_overlay(w, h);
  overlay_time.copy_overlay(bg_overlay, x, y);
  overlay_time.set_text(0, 0, text, text_size, 0x00f4419d);
  overlay_time.print_to_display(x, y);
}

void handle_keyboard_input(int &exit_code, bool &pause) {
  int key = getchar();
  switch (key) {
    case 27:  // ESC
    {
      int next_key = getchar();
      switch (next_key) {
        case 91:  // there are more value to read: UP/DOWN/LEFT/RIGHT pressed
          break;
        case 79:  // F3 pressed
          break;
        default:  // nothing special was pressed, will exit
          exit_code = 0;
          break;
      }
      break;
    }
    case '1':
    case 32:  // SPACE
      pause = !pause;
      break;
  }
}
