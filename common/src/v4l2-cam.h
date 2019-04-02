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

#pragma once

int v4l2_cam_init(int width, int height, int fps);

int v4l2_cam_start(void);

int v4l2_cam_get(unsigned int* __restrict vImg, int capture_w, int capture_h,
                 int crop_xoffs, int crop_yoffs, int crop_w, int crop_h);

int v4l2_cam_stop(void);

int v4l2_cam_deinit(void);
