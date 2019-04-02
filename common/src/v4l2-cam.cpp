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

#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdint.h>

#include <linux/videodev2.h>


#define CLIPVALUE(val) (val > 255 ? 255 : val < 0 ? 0 : val)

static inline void YUV2RGB(const unsigned char y, const unsigned char u,
                           const unsigned char v, unsigned int *rgb) {
  const int y2 = (int)y;
  const int u2 = (int)u - 128;
  const int v2 = (int)v - 128;
  int r = y2 + ((v2 * 91947) >> 16);
  int g = y2 - (((u2 * 22544) + (v2 * 46793)) >> 16);
  int b = y2 + ((u2 * 115999) >> 16);
  r = CLIPVALUE(r);
  g = CLIPVALUE(g);
  b = CLIPVALUE(b);
  *rgb = (r << 24) | (g << 16) | (b << 8);
}

static void yuyv2rgb_crop(unsigned char *__restrict yuyv,
                          unsigned int *__restrict vImg, int capture_w,
                          int capture_h, int crop_xoffs, int crop_yoffs,
                          int crop_w, int crop_h) {
  assert(((capture_w & 1) == 0) && ((crop_xoffs & 1) == 0) &&
         ((crop_w & 1) == 0));
  assert(crop_xoffs + crop_w <= capture_w);
  assert(crop_yoffs + crop_h <= capture_h);

  unsigned char y0, y1, u, v;
  unsigned int rgb;

  int i = 2 * (crop_yoffs * capture_w + crop_xoffs);
  int j = 0;

  for (int y = crop_yoffs; y < (crop_yoffs + crop_h); y++) {
    for (int x = crop_xoffs; x < (crop_xoffs + crop_w); x += 2, i += 4, j += 2) {
      y0 = (unsigned char)yuyv[i];
      u = (unsigned char)yuyv[i + 1];
      y1 = (unsigned char)yuyv[i + 2];
      v = (unsigned char)yuyv[i + 3];
      YUV2RGB(y0, u, v, &rgb);
      vImg[j] = rgb;
      YUV2RGB(y1, u, v, &rgb);
      vImg[j + 1] = rgb;
    }
    i += 2 * (capture_w - crop_w);
  }
}

static void uyvy2rgb_crop(unsigned char *__restrict uyvy,
                          unsigned int *__restrict vImg, int capture_w,
                          int capture_h, int crop_xoffs, int crop_yoffs,
                          int crop_w, int crop_h) {
  assert(((capture_w & 1) == 0) && ((crop_xoffs & 1) == 0) &&
         ((crop_w & 1) == 0));
  assert(crop_xoffs + crop_w <= capture_w);
  assert(crop_yoffs + crop_h <= capture_h);

  unsigned char y0, y1, u, v;
  unsigned int rgb;

  int i = 2 * (crop_yoffs * capture_w + crop_xoffs);
  int j = 0;

  for (int y = crop_yoffs; y < (crop_yoffs + crop_h); y++) {
    for (int x = crop_xoffs; x < (crop_xoffs + crop_w); x += 2, i += 4, j += 2) {
      u = (unsigned char)uyvy[i];
      y0 = (unsigned char)uyvy[i + 1];
      v = (unsigned char)uyvy[i + 2];
      y1 = (unsigned char)uyvy[i + 3];
      YUV2RGB(y0, u, v, &rgb);
      vImg[j] = rgb;
      YUV2RGB(y1, u, v, &rgb);
      vImg[j + 1] = rgb;
    }
    i += 2 * (capture_w - crop_w);
  }
}


static int fd = -1;
static char dev_name[] = "/dev/video0";
struct buffer {
  void *start;
  size_t length;
  int index;
};
struct buffer *buffers;
static unsigned int n_buffers;
typedef void (*f_crop)(unsigned char *__restrict yuv,
                       unsigned int *__restrict vImg, int capture_w,
                       int capture_h, int crop_xoffs, int crop_yoffs,
                       int crop_w, int crop_h);
static f_crop g_crop = yuyv2rgb_crop;


static int v4l2_cam_devinit(int width, int height, int fps) {
  struct v4l2_capability cap;
  struct v4l2_cropcap cropcap;
  struct v4l2_crop crop;
  struct v4l2_format fmt;
  struct v4l2_requestbuffers req;
  struct v4l2_streamparm strm;

  if (ioctl(fd, VIDIOC_QUERYCAP, &cap) == -1) {
    fprintf(stderr, "VIDIOC_QUERYCAP error\n");
    return -1;
  }

  if (!(cap.capabilities & V4L2_CAP_VIDEO_CAPTURE)) {
    fprintf(stderr, "%s is not video capture device\n", dev_name);
    return -1;
  }
  if (!(cap.capabilities & V4L2_CAP_STREAMING)) {
    fprintf(stderr, "%s does not support streaming i/o\n", dev_name);
    return -1;
  }

  memset(&cropcap, 0, sizeof(cropcap));
  cropcap.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

  if (ioctl(fd, VIDIOC_CROPCAP, &cropcap) == 0) {
    crop.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    crop.c = cropcap.defrect;
    if (ioctl(fd, VIDIOC_S_CROP, &crop) == -1) {
      /* Cropping not supported. */
    }
  } else {
    /* Cropping not supported. */
  }

  memset(&fmt, 0, sizeof(fmt));
  fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  fmt.fmt.pix.width = width;
  fmt.fmt.pix.height = height;
  const uint32_t ipix = V4L2_PIX_FMT_UYVY;
  fmt.fmt.pix.pixelformat = ipix;
  fmt.fmt.pix.field = V4L2_FIELD_ANY;

  if (ioctl(fd, VIDIOC_S_FMT, &fmt) < 0) {
    fprintf(stderr, "%s does not support V4L2_PIX_FMT_UYVY format\n", dev_name);
    return -1;
  }

  if (ioctl(fd, VIDIOC_G_FMT, &fmt) < 0) {
    fprintf(stderr, "%s does not support V4L2_PIX_FMT_UYVY format\n", dev_name);
    return -1;
  }

  const uint32_t opix = fmt.fmt.pix.pixelformat;

  bool bad_pix = false;
  switch (opix) {
    case V4L2_PIX_FMT_UYVY:
      g_crop = uyvy2rgb_crop;
      break;

    case V4L2_PIX_FMT_YUYV:
      g_crop = yuyv2rgb_crop;
      break;

    default:
      bad_pix = true;
      break;
  }

  if ((bad_pix) || ((int)fmt.fmt.pix.width != width) || ((int)fmt.fmt.pix.height != height)) {
    fprintf(stderr, "Requested width=%d height=%d pixelformat=%c%c%c%c but got width=%d height=%d pixelformat=%c%c%c%c\n",
            width, height,
            (char)(ipix & 0xFF), (char)(ipix >> 8), (char)((ipix >> 16) & 0xFF), (char)((ipix >> 24) & 0xFF),
            (int)fmt.fmt.pix.width, (int)fmt.fmt.pix.height,
            (char)(opix & 0xFF), (char)(opix >> 8), (char)((opix >> 16) & 0xFF), (char)((opix >> 24) & 0xFF));
    return -1;
  }

  if (fps > 0) {
    memset(&strm, 0, sizeof(strm));
    strm.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    if (ioctl(fd, VIDIOC_G_PARM, &strm) == -1) {
      fprintf(stderr, "%s could not get streaming params\n", dev_name);
      return -1;
    }

    strm.parm.capture.capturemode |= V4L2_CAP_TIMEPERFRAME;
    strm.parm.capture.timeperframe.numerator = 1;
    strm.parm.capture.timeperframe.denominator = fps;

    if (ioctl(fd, VIDIOC_S_PARM, &strm) == -1) {
      fprintf(stderr, "%s could not set streaming params\n", dev_name);
      return -1;
    }
  }

  memset(&req, 0, sizeof(req));
  req.count = 4;
  req.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  req.memory = V4L2_MEMORY_MMAP;

  if (ioctl(fd, VIDIOC_REQBUFS, &req) == -1) {
    fprintf(stderr, "%s does not support memory mapping\n", dev_name);
    return -1;
  }

  buffers = (buffer *)calloc(req.count, sizeof(*buffers));
  if (!buffers) {
    fprintf(stderr, "out of memory\n");
    return -1;
  }

  for (n_buffers = 0; n_buffers < req.count; ++n_buffers) {
    struct v4l2_buffer buf;
    memset(&buf, 0, sizeof(buf));
    buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    buf.memory = V4L2_MEMORY_MMAP;
    buf.index = n_buffers;
    if (ioctl(fd, VIDIOC_QUERYBUF, &buf) < 0) {
      fprintf(stderr, "VIDIOC_QUERYBUF error\n");
      return -1;
    }
    //fprintf(stderr, "buffer %d @ 0x%08x L=%d\n", n_buffers, buf.m.offset,
    //        buf.length);
    buffers[n_buffers].length = buf.length;
    buffers[n_buffers].start = mmap(NULL, buf.length, PROT_READ | PROT_WRITE,
                                    MAP_SHARED, fd, buf.m.offset);
    if (MAP_FAILED == buffers[n_buffers].start) {
      fprintf(stderr, "mmap error\n");
      return -1;
    }
  }
  return 0;
}

int v4l2_cam_init(int width, int height, int fps) {
  fd = open(dev_name, O_RDWR | O_NONBLOCK, 0);
  if (fd == -1) {
    fprintf(stderr, "Cannot open '%s': %d, %s\n", dev_name, errno,
            strerror(errno));
    return -1;
  }

  return v4l2_cam_devinit(width, height, fps);
}

int v4l2_cam_start(void) {
  enum v4l2_buf_type type;

  for (unsigned int i = 0; i < n_buffers; ++i) {
    struct v4l2_buffer buf;
    memset(&buf, 0, sizeof(buf));
    buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    buf.memory = V4L2_MEMORY_MMAP;
    buf.index = i;
    if (ioctl(fd, VIDIOC_QBUF, &buf) == -1) {
      fprintf(stderr, "VIDIOC_QBUF (cam_start)error %d %s\n", errno,
              strerror(errno));
      return -1;
    }
  }
  type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  if (ioctl(fd, VIDIOC_STREAMON, &type) == -1) {
    fprintf(stderr, "VIDIOC_STREAMON error\n");
    return -1;
  }

  return 0;
}

int v4l2_cam_get(unsigned int *__restrict vImg, int capture_w, int capture_h,
                 int crop_xoffs, int crop_yoffs, int crop_w, int crop_h) {
  fd_set fds;
  struct timeval tv;
  int ret;
  struct v4l2_buffer buf;

  FD_ZERO(&fds);
  FD_SET(fd, &fds);

  /* Timeout. */
  tv.tv_sec = 2;
  tv.tv_usec = 0;

  ret = select(fd + 1, &fds, NULL, NULL, &tv);
  if (ret == -1) {
    fprintf(stderr, "select() failed for camera\n");
    return -1;
  }

  if (ret == 0) {
    fprintf(stderr, "select() timeouted for camera\n");
    return -1;
  }

  memset(&buf, 0, sizeof(buf));
  buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  buf.memory = V4L2_MEMORY_MMAP;
  if (ioctl(fd, VIDIOC_DQBUF, &buf) == -1) {
    fprintf(stderr, "VIDIOC_DQBUF error\n");
    return -1;
  }

  assert(buf.index < n_buffers);

  (*g_crop)((unsigned char *)buffers[buf.index].start, vImg, capture_w,
            capture_h, crop_xoffs, crop_yoffs, crop_w, crop_h);

  if (ioctl(fd, VIDIOC_QBUF, &buf) == -1) {
    fprintf(stderr, "VIDIOC_QBUF (cam_get)error\n");
    return -1;
  }
  return 0;
}

int v4l2_cam_stop() {
  if (fd == -1) {
    return 0;
  }
  enum v4l2_buf_type type;
  type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  if (ioctl(fd, VIDIOC_STREAMOFF, &type) == -1) {
    fprintf(stderr, "VIDIOC_STREAMOFF error\n");
    return -1;
  }
  return 0;
}

static int v4l2_cam_devdeinit(void) {
  for (unsigned int i = 0; i < n_buffers; ++i) {
    if (munmap(buffers[i].start, buffers[i].length) == -1) {
      fprintf(stderr, "munmap error\n");
      return -1;
    }
  }
  return 0;
}

int v4l2_cam_deinit(void) {
  if (fd == -1) {
    return 0;
  }
  v4l2_cam_devdeinit();
  if (close(fd) == -1) {
    fprintf(stderr, "close() failed for camera\n");
    return -1;
  }
  fd = -1;
  return 0;
}
