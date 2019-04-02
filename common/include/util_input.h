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

#ifndef __INPUT_UTIL
#define __INPUT_UTIL

#include <dirent.h>
#include <stdio.h>
#include <algorithm>
#include <string>
#include <vector>

namespace dmp {
namespace util {

int open_cam(int capture_w, int capture_h, int fps);
void close_cam();
int capture_cam(uint32_t* vImg, int capture_w, int capture_h,
                int crop_xoffs, int crop_yoffs, int crop_w, int crop_h);

/*! Utility function for returning all the filenames in a folder with specific
* extension
* @param[in] dir_name Folder to look into
* @param[in] suffix_list List of extensions
* @param[return] vector containing filenames in folder with specified extension
*/
std::vector<std::string> get_input_image_names(
    const std::string &dir_name, const std::vector<std::string> &suffix_list);

/*! Utility function for returning all the filenames in a folder with specific
* extension. Width and height must match.
* @param[in] imgFileName jpeg filename
* @param[in] imgView allocated memory for storing the decoded jpeg image (32
* bits)
* @param[in] img_width image width
* @param[in] img_height image height
*/
int decode_jpg_file(const std::string &imgFileName, uint32_t *imgView,
                    int img_width, int img_height);

/*! Utility function for preprocessing images and preparing the network input.
* Typically, each r,g and b components are first offset, then scaled: c =
* (c+offset)*scale.
* @param[in] imgView pointer to memory containing the input image
* @param[in] img_width image width
* @param[in] img_height image height
* @param[in] r_offs red channel offset
* @param[in] g_offs green channel offset
* @param[in] b_offs blue channel offset
* @param[in] sf scale factor
* @param[in] transpose if enabled, transpose the image (switch width/height).
* @param[in] is_bgr if true, the image will be converted to bgr order, else will
* remain in rgb order.
* @param[out] imgProc pointer to memory containing the output image, used as
* input of the network (float 16 bits)
*/
void preproc_image(uint32_t *__restrict imgView, __fp16 *__restrict imgProc,
                   int img_width, int img_height, float r_offs, float g_offs,
                   float b_offs, float sf, bool transpose, bool is_bgr = true);

/*! Utility function for preprocessing images and preparing the network input.
* Will scale the image first. Then process the image.
* @param[in] imgView pointer to memory containing the input image
* @param[in] inimg_width input image width
* @param[in] inimg_height input image height
* @param[in] outimg_width output image width
* @param[in] outimg_height output image height
* @param[in] r_offs red channel offset
* @param[in] g_offs green channel offset
* @param[in] b_offs blue channel offset
* @param[in] sf scale factor
* @param[in] transpose if enabled, transpose the image (switch width/height).
* @param[in] is_bgr if true, the image will be converted to bgr order, else will
* remain in rgb order.
* @param[out] imgProc pointer to memory containing the output image, used as
* input of the network (float 16 bits)
*/
void preproc_image(uint32_t *__restrict imgView, __fp16 *__restrict imgProc,
                   int inimg_width, int inimg_height, int outimg_width,
                   int outimg_height, float r_offs, float g_offs, float b_offs,
                   float sf, bool transpose, bool is_bgr = true);

};  // end of namespace util
};  // end of namespace dmp

#endif  //__INPUT_UTIL
