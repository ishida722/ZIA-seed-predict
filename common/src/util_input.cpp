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

#include <stdint.h>
#include <cstring>

#include "util_input.h"
#include "nanojpeg.h"
#include "v4l2-cam.h"

namespace dmp {
namespace util {

int open_cam(int capture_w, int capture_h, int fps) {
  int res = v4l2_cam_init(capture_w, capture_h, fps);
  if (res) {
    return res;
  }
  res = v4l2_cam_start();
  if (res) {
    return res;
  }
  return 0;
}

void close_cam() {
  v4l2_cam_stop();
  v4l2_cam_deinit();
}

int capture_cam(uint32_t *vImg, int capture_w, int capture_h,
                 int crop_xoffs, int crop_yoffs, int crop_w, int crop_h) {
  return v4l2_cam_get(vImg, capture_w, capture_h, crop_xoffs, crop_yoffs, crop_w, crop_h);
}

bool has_suffix(const std::string &str, const std::string &suffix) {
  return str.size() >= suffix.size() &&
         str.compare(str.size() - suffix.size(), suffix.size(), suffix) == 0;
}

std::vector<std::string> get_input_image_names(
    const std::string &dir_name, const std::vector<std::string> &suffix_list) {
  std::vector<std::string> image_names;

  DIR *dirp;
  struct dirent *directory;

  dirp = opendir(dir_name.c_str());

  if (dirp) {
    while ((directory = readdir(dirp)) != NULL) {
#ifdef _DIRENT_HAVE_D_TYPE
      if (directory->d_type == DT_REG || directory->d_type == DT_UNKNOWN) {
#endif
        std::string image_name = std::string(directory->d_name);
        for (size_t i = 0; i < suffix_list.size(); i++)
          if(has_suffix(image_name, suffix_list[i])) {
            image_names.push_back(dir_name + image_name);
            break;
          }
#ifdef _DIRENT_HAVE_D_TYPE
      } else if (directory->d_type == DT_DIR) {
        if (strcmp(directory->d_name, ".") != 0
            && strcmp(directory->d_name, "..") != 0) {
          std::vector<std::string> v = get_input_image_names(dir_name 
              + std::string("/")
              + directory->d_name
              + std::string("/")
              , suffix_list);
          image_names.insert(image_names.end(), v.begin(), v.end());
        }
      }
#endif
    }
    closedir(dirp);
  }

  std::sort(image_names.begin(), image_names.end());

  return image_names;
}

int decode_jpg_file(const std::string &imgFileName, uint32_t *imgView,
                    int img_width, int img_height) {
  int size;
  char *buf;
  FILE *f;
  f = fopen(imgFileName.c_str(), "rb");
  if (!f) {
    printf("Error opening input image file.\n");
    return 1;
  }
  fseek(f, 0, SEEK_END);
  size = (int)ftell(f);
  buf = (char *)malloc(size);
  fseek(f, 0, SEEK_SET);
  size = (int)fread(buf, 1, size, f);
  fclose(f);

  njInit();
  if (njDecode(buf, size)) {
    free((void *)buf);
    printf("Error decoding the input image file.\n");
    return 1;
  }
  free((void *)buf);

  unsigned char *rgb = njGetImage();
  if (njIsColor()) {
    if (njGetImageSize() != img_width * img_height * 3) {
      printf("Incorrect input image size.\n");
      njDone();
      return 1;
    }
    int i = 0;
    for (int j = 0; j < img_width * img_height; j++) {
      unsigned char r = rgb[i];
      i++;
      unsigned char g = rgb[i];
      i++;
      unsigned char b = rgb[i];
      i++;
      imgView[j] = (r << 24) | (g << 16) | (b << 8);
    }
  } else {
    if (njGetImageSize() != img_width * img_height) {
      printf("Incorrect input image size.\n");
      njDone();
      return 1;
    }
    int i = 0;
    for (int j = 0; j < img_width * img_height; j++) {
      unsigned char g = rgb[i];
      i++;
      imgView[j] = (g << 24) | (g << 16) | (g << 8);
    }
  }
  njDone();

  return 0;
}


void preproc_image(uint32_t *__restrict imgView, __fp16 *__restrict imgProc,
                   int img_width, int img_height, float r_offs, float g_offs,
                   float b_offs, float sf, bool transpose, bool is_bgr) {
  int i = 0, j = 0;
  for (int y = 0; y < img_height; y++) {
    if (transpose) j = 3 * y;
    for (int x = 0; x < img_width; x++) {
      uint32_t rgba = imgView[i];
      i++;
      unsigned char r = (rgba >> 24);
      unsigned char g = (rgba >> 16) & 0xFF;
      unsigned char b = (rgba >> 8) & 0xFF;
      if (is_bgr) {
        imgProc[j] = (__fp16)(sf * (b_offs + b));
        j++;
        imgProc[j] = (__fp16)(sf * (g_offs + g));
        j++;
        imgProc[j] = (__fp16)(sf * (r_offs + r));
        j++;
      } else {
        imgProc[j] = (__fp16)(sf * (r_offs + r));
        j++;
        imgProc[j] = (__fp16)(sf * (g_offs + g));
        j++;
        imgProc[j] = (__fp16)(sf * (b_offs + b));
        j++;
      }
      if (transpose) j += 3 * (img_height - 1);
    }
  }
}


void preproc_image(uint32_t *__restrict imgView, __fp16 *__restrict imgProc,
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
  preproc_image(imgIn, imgProc, outimg_width, outimg_height, r_offs, g_offs,
                b_offs, sf, transpose, is_bgr);
  if (imgIn != imgView) delete[] imgIn;
}

};  // end of namespace util
};  // end of namespace dmp
