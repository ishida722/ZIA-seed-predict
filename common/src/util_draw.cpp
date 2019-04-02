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

#include "util_draw.h"

#include <stdio.h>
#include <math.h>

#include <linux/fb.h>
#include <linux/videodev2.h>
#include <linux/kd.h>
#include <signal.h>
#include <termios.h>

namespace dmp {
namespace util {

static int g_fb_file = -1;
static struct fb_fix_screeninfo g_fb_fix_info;
static struct fb_var_screeninfo g_fb_var_info;
static uint32_t g_fb_pixfmt = 0;
static uint8_t *g_fb_mem = NULL;
static uint8_t *g_frame_ptr = NULL;
static int g_console = -1;
static struct termios g_old_term;
static bool g_term_set = false;

#define SCREEN_W (g_fb_var_info.xres)
#define SCREEN_H (g_fb_var_info.yres)

#define ERR(...) fprintf(stderr, __VA_ARGS__); fflush(stderr)

static void update_frame_ptr() {
  if (g_fb_var_info.yoffset) {
    g_frame_ptr = g_fb_mem;
  }
  else {
    g_frame_ptr = g_fb_mem + SCREEN_H * SCREEN_W * (g_fb_var_info.bits_per_pixel >> 3);
  }
}

static bool set_pan(uint32_t pan_x, uint32_t pan_y) {
  struct fb_var_screeninfo var_info;
  memset(&var_info, 0, sizeof var_info);
  var_info.xoffset = pan_x;
  var_info.yoffset = pan_y;

  int res = ioctl(g_fb_file, FBIOPAN_DISPLAY, &var_info);
  if (res < 0) {
    ERR("ioctl(FBIOPAN_DISPLAY) failed for /dev/fb0\n");
    return false;
  }

  g_fb_var_info.xoffset = var_info.xoffset;
  g_fb_var_info.yoffset = var_info.yoffset;

  update_frame_ptr();

  return true;
}

static inline uint8_t* get_frame_ptr() {
  return g_frame_ptr;
}

uint32_t get_screen_width() {
  return g_fb_var_info.xres;
}

uint32_t get_screen_height() {
  return g_fb_var_info.yres;
}

static void release_console() {
  if (g_term_set) {
    while (getchar() != -1) {  // read cached characters
    }
    tcsetattr(0, TCSANOW, &g_old_term);
    g_term_set = false;
  }
  if (g_console != -1) {
    ioctl(g_console, KDSETMODE, KD_TEXT);
    close(g_console);
    g_console = -1;
  }
}

static void release_fb() {
  release_console();
  if (g_fb_mem) {
    munmap(g_fb_mem, g_fb_fix_info.smem_len);
    g_fb_mem = NULL;
  }
  if (g_fb_file != -1) {
    close(g_fb_file);
    g_fb_file = -1;
  }
}

/// @brief Previous signal handlers.
static void (*g_prev_sigint)(int sig) = NULL;
static void (*g_prev_sigquit)(int sig) = NULL;
static void (*g_prev_sigterm)(int sig) = NULL;

static void process_signal(int sig, void (*prev_handler)(int sig)) {
  release_console();
  if (prev_handler == SIG_IGN) {
    return;
  }
  if (prev_handler == SIG_DFL) {
    signal(sig, SIG_DFL);
    raise(sig);
    return;
  }
  (*prev_handler)(sig);
}

static void on_sigint(int sig) {
  process_signal(sig, g_prev_sigint);
}

static void on_sigquit(int sig) {
  process_signal(sig, g_prev_sigquit);
}

static void on_sigterm(int sig) {
  process_signal(sig, g_prev_sigterm);
}

bool init_fb() {
  if (g_fb_file != -1) {
    ERR("Framebuffer is already opened\n");
    return false;
  }
  g_fb_file = open("/dev/fb0", O_RDWR | O_CLOEXEC);
  if (g_fb_file == -1) {
    ERR("open() failed for /dev/fb0\n");
    return false;
  }

  int res = ioctl(g_fb_file, FBIOGET_FSCREENINFO, &g_fb_fix_info);
  if (res < 0) {
    ERR("ioctl(FBIOGET_FSCREENINFO) failed for /dev/fb0\n");
    release_fb();
    return false;
  }
  res = ioctl(g_fb_file, FBIOGET_VSCREENINFO, &g_fb_var_info);
  if (res < 0) {
    ERR("ioctl(FBIOGET_FSCREENINFO) failed for /dev/fb0\n");
    release_fb();
    return false;
  }
  if ((!g_fb_var_info.xres) || (!g_fb_var_info.yres)) {
    ERR("Could not determine framebuffer dimensions: xres=%u yres=%u\n",
        g_fb_var_info.xres, g_fb_var_info.yres);
    release_fb();
    return false;
  }

  switch (g_fb_var_info.bits_per_pixel) {
    case 16:
      g_fb_pixfmt = V4L2_PIX_FMT_RGB565;
      break;
    case 24:
      if (g_fb_var_info.red.offset == 0) {
        g_fb_pixfmt = V4L2_PIX_FMT_RGB24;
      }
      else {
        g_fb_pixfmt = V4L2_PIX_FMT_BGR24;
      }
      break;
     case 32:
      if (g_fb_var_info.red.offset == 0) {
        g_fb_pixfmt = V4L2_PIX_FMT_RGB32;
      }
      else {
        g_fb_pixfmt = V4L2_PIX_FMT_BGR32;
      }
      break;
      default: {
        g_fb_pixfmt = 0;
      }
  }
  if (g_fb_pixfmt != V4L2_PIX_FMT_BGR24) {
    ERR("Unsupported pixel format: bpp=%d red.offset=%d\n",
        g_fb_var_info.bits_per_pixel, g_fb_var_info.red.offset);
    release_fb();
    return false;
  }

  if (g_fb_fix_info.smem_len < g_fb_var_info.xres * g_fb_var_info.yres * (g_fb_var_info.bits_per_pixel >> 2)) {
    ERR("Framebuffer doesn't support double buffering\n");
    release_fb();
    return false;
  }

  if (g_fb_fix_info.line_length != g_fb_var_info.xres * (g_fb_var_info.bits_per_pixel >> 3)) {
    ERR("Support for framebuffer with bigger than visible width %d line length %d is not implemented\n",
        g_fb_var_info.xres * (g_fb_var_info.bits_per_pixel >> 3), g_fb_fix_info.line_length);
    release_fb();
    return false;
  }

  g_fb_mem = (uint8_t*)mmap(
      NULL, g_fb_fix_info.smem_len, PROT_READ | PROT_WRITE,
      MAP_SHARED, g_fb_file, 0);
  if (!g_fb_mem) {
    ERR("mmap() failed for /dev/fb0\n");
    release_fb();
    return false;
  }

  // Set signal handlers for restoring console settings
  g_prev_sigint = signal(SIGINT, on_sigint);
  g_prev_sigquit = signal(SIGQUIT, on_sigquit);
  g_prev_sigterm = signal(SIGTERM, on_sigterm);
  atexit(release_console);

  // Set graphics mode on the console
  long mode = 0;
  g_console = open("/dev/tty0", O_RDWR | O_CLOEXEC);
  if (g_console != -1) {
    if (ioctl(g_console, KDGETMODE, &mode) < 0) {
      ERR("Could not determine console mode (text or graphics)\n");
      close(g_console);
      g_console = -1;
    }
  }
  if (g_console != -1) {
    if (mode == KD_TEXT) {  // text mode => graphics mode
      if (ioctl(g_console, KDSETMODE, KD_GRAPHICS) < 0) {
        ERR("Could not change console to graphics mode\n");
        close(g_console);
        g_console = -1;
      }
    }
    else {  // already in graphics mode
      close(g_console);
      g_console = -1;
    }
  }

  // Setup console to read one character at a time
  struct termios new_term;
  tcgetattr(0, &g_old_term);
  memcpy(&new_term, &g_old_term, sizeof(new_term));
  new_term.c_lflag &= ~ICANON;
  new_term.c_lflag &= ~ECHO;
  new_term.c_lflag &= ~ISIG;
  new_term.c_cc[VMIN] = 0;
  new_term.c_cc[VTIME] = 0;
  tcsetattr(0, TCSANOW, &new_term);
  g_term_set = true;

  // reset screen pan
  set_pan(g_fb_var_info.xoffset, 0);

  return true;
}

void shutdown() {
  release_fb();
}

bool swap_buffer() {
  int res = ioctl(g_fb_file, FBIOBLANK, FB_BLANK_UNBLANK);
  if (res < 0) {
    ERR("ioctl(FB_BLANK_UNBLANK) failed for /dev/fb0\n");
    return false;
  }

  unsigned int screen = 0;
  res = ioctl(g_fb_file, FBIO_WAITFORVSYNC, &screen);
  if (res < 0) {
    ERR("ioctl(FBIO_WAITFORVSYNC) failed for /dev/fb0\n");
    return false;
  }

  return set_pan(g_fb_var_info.xoffset, g_fb_var_info.yoffset ? 0 : g_fb_var_info.yres);
}

void print_result(const std::vector<std::string>& catstr_vec,
                  int xpos, int ypos, const std::vector<std::pair<float, int> >& f, 
                  COverlayRGB &bg_overlay) {
  std::vector<std::pair<std::string, float> > result;
  result.push_back(std::make_pair(catstr_vec[f[0].second], f[0].first));
  result.push_back(std::make_pair(catstr_vec[f[1].second], f[1].first));
  result.push_back(std::make_pair(catstr_vec[f[2].second], f[2].first));
  result.push_back(std::make_pair(catstr_vec[f[3].second], f[3].first));
  result.push_back(std::make_pair(catstr_vec[f[4].second], f[4].first));

  std::stringstream ss;
  std::string s;
  for (uint32_t i = 0; i < result.size(); i++) {
    std::pair<std::string, float> p = result[i];
    ss.str("");
    ss << std::fixed << std::setprecision(3) << p.second << "  " << p.first;
    s = ss.str();
    if (s.size() > 45) s = s.substr(0, 45 - 3) + "...";
    s.resize(45, ' ');
    if (i == 0)
    {
      unsigned text_size = 12;
      unsigned w = 0;
      unsigned h = 0;
      COverlayRGB::calculate_boundary_text(s, text_size, w, h);
      COverlayRGB predict_text(SCREEN_W, SCREEN_H);
      predict_text.alloc_mem_overlay(w, h);

      int x = xpos;
      int y = ypos;

      //clear previous text
      COverlayRGB clear_bg(SCREEN_W, SCREEN_H);
      clear_bg.alloc_mem_overlay(SCREEN_W, h);
      clear_bg.copy_overlay(bg_overlay, 0, y);
      clear_bg.print_to_display(0, y);

      predict_text.copy_overlay(bg_overlay,x, y);
      predict_text.set_text(0, 0, s, text_size, 0x0042f4eb);
      predict_text.print_to_display(x, y);
    }
    else
    {
      unsigned text_size = 12;
      unsigned w = 0;
      unsigned h = 0;
      COverlayRGB::calculate_boundary_text(s, text_size, w, h);
      COverlayRGB predict_text(SCREEN_W, SCREEN_H);
      predict_text.alloc_mem_overlay(w, h);

      int x = xpos;
      int y = ypos + 30*i;

      //clear previous text
      COverlayRGB clear_bg(SCREEN_W, SCREEN_H);
      clear_bg.alloc_mem_overlay(SCREEN_W, h);
      clear_bg.copy_overlay(bg_overlay, 0, y);
      clear_bg.print_to_display(0, y);

      predict_text.copy_overlay(bg_overlay,x, y);
      predict_text.set_text(0, 0, s, text_size, 0x00c8f442);
      predict_text.print_to_display(x, y);
    }
    if (i == 0) 
    {
      std::string all_words = p.first;
      std::string word = all_words.substr(0, all_words.find(","));
      if (word.size() > 32) word = word.substr(0, 32 - 3) + "...";

      unsigned text_size = 20;
      unsigned w = 0;
      unsigned h = 0;
      COverlayRGB::calculate_boundary_text(word, text_size, w, h);
      COverlayRGB predict_text(SCREEN_W, SCREEN_H);
      predict_text.alloc_mem_overlay(w, h);

      int x = ((SCREEN_W - w) / 2);
      int y = ypos-40;

      //clear previous text
      COverlayRGB clear_bg(SCREEN_W, SCREEN_H);
      clear_bg.alloc_mem_overlay(SCREEN_W, h);
      clear_bg.copy_overlay(bg_overlay, 0, y);
      clear_bg.print_to_display(0, y);

      predict_text.copy_overlay(bg_overlay,x, y);
      predict_text.set_text(0, 0, word, text_size, 0x00f9c252);
      predict_text.print_to_display(x, y);
    }
  }
}

std::vector<std::pair<float, int> > catrank(float* softmax, int count) {
  std::vector<std::pair<float, int> > v;
  for (int i = 0; i < count; i++) {
    std::pair<float, int> p;
    p.first = softmax[i];
    p.second = i;
    v.push_back(p);
  }
  std::sort(v.begin(), v.end());
  std::reverse(v.begin(), v.end());

  std::vector<std::pair<float, int> > r(v.begin(), v.begin() + 5);
  return r;
}

COverlayRGB::COverlayRGB(uint32_t screen_width, uint32_t screen_height)
{
  if((screen_width > 0) && (screen_height > 0) )
  {
    screen_width_ = screen_width;
    screen_height_ = screen_height;
    pixel_size_ = 3;//rgb24
  }
}

void COverlayRGB::alloc_mem_overlay(uint32_t overlay_width,
                                    uint32_t overlay_height)
{
  overlay_rgb_width_ = overlay_width;
  overlay_rgb_height_ = overlay_height;
  if(buff_rgb_ != NULL)
  {
    delete [] buff_rgb_;
    buff_rgb_ = NULL;
  }   
  buff_rgb_ = new unsigned char[overlay_rgb_width_*overlay_rgb_height_*pixel_size_];
  if(buff_rgb_ != NULL)
  {
    memset(buff_rgb_, 0, overlay_rgb_width_*overlay_rgb_height_*pixel_size_);
    render_buf_overlay_rgb_.attach(buff_rgb_,
                                    overlay_rgb_width_,
                                    overlay_rgb_height_,
                                    overlay_rgb_width_ * pixel_size_);
  }
  else
  {
    ERR("failed to allocate memory for overlay\n" );
  }
}

uint8_t* COverlayRGB::get_overlay_buf_ref(void)
{
    return buff_rgb_;
}

bool COverlayRGB::convert_to_overlay_pixel_format(uint32_t *imgview, uint32_t size_of_imgview)
{
  if((imgview != NULL) && (size_of_imgview == overlay_rgb_width_*overlay_rgb_height_)
            && (buff_rgb_ != NULL))
  {
    unsigned char *p = this->get_overlay_buf_ref();
    for(unsigned int i = 0; i < size_of_imgview; i++)
    {
      p[pixel_size_*i] = (imgview[i]&0xff000000)>>24;
      p[pixel_size_*i+1] = (imgview[i]&0x00ff0000)>>16;
      p[pixel_size_*i+2] = (imgview[i]&0x0000ff00)>>8;
    }
    return true;
  }
  else
  {
    ERR("convert_to_overlay_pixel_format() failed\n" );
  }
  return false;
}

rbuf_type& COverlayRGB::get_ren_buf_ref(void)
{
    return render_buf_overlay_rgb_;
}

uint8_t* COverlayRGB::get_overlay_row_ptr_ref(uint32_t row)
{
    return render_buf_overlay_rgb_.row_ptr(row);
}

bool COverlayRGB::set_pixel(uint32_t xpos, uint32_t ypos, uint8_t red, uint8_t green, uint8_t blue)
{
  if((xpos <= overlay_rgb_width_) && (ypos <= overlay_rgb_height_) && (buff_rgb_ != NULL))
  {
    unsigned char *p = render_buf_overlay_rgb_.row_ptr(ypos);
    unsigned int j = xpos * pixel_size_;
    p[j] = red;
    j++;
    p[j] = green;
    j++;
    p[j] = blue;
    return true;
  }
  else
  {
    ERR("set_pixel() failed\n" );
  }
  return false;
}

bool COverlayRGB::copy_overlay(COverlayRGB &src_overlay, uint32_t xpos, uint32_t ypos)
{
  uint32_t dst_row = 0;
  uint32_t w = xpos + overlay_rgb_width_;
  uint32_t h = ypos + overlay_rgb_height_;
  if((w <= src_overlay.get_overlay_width()) &&
        (h <= src_overlay.get_overlay_height()) && (buff_rgb_ != NULL))
  {
    for(uint32_t row = ypos; row < (h); row++)
    {
      uint8_t *p = render_buf_overlay_rgb_.row_ptr(dst_row);
      uint8_t *psrc = src_overlay.get_overlay_row_ptr_ref(row);
      psrc = psrc + xpos * pixel_size_;
      memcpy(p, psrc, pixel_size_*(w-xpos));
      dst_row++;
    }
    return true;
  }
  else
  {
    ERR("copy_overlay() failed\n" );
  }
  return false;
}
void COverlayRGB::set_box_with_text(int32_t x0pos, int32_t y0pos, int32_t x1pos, 
                                        int32_t y1pos, uint32_t color, string text)
{
  static const int32_t text_size = 8;

  if(buff_rgb_ != NULL)
  {
    pixfmt_type pixf(render_buf_overlay_rgb_);
    base_ren_type rb(pixf);

    agg::scanline_u8 scanLine;
    agg::rasterizer_scanline_aa<> ras;

    //draw big box
    renderer_solid rs_box(rb);
    rs_box.color(agg::rgba8((color & 0x00ff0000) >> 16,
          (color & 0x0000ff00) >>  8 , (color & 0x000000ff)));
    agg::rounded_rect rec(x0pos, y0pos, x1pos, y1pos, 0);
    agg::conv_stroke<agg::rounded_rect> stroke(rec);
    stroke.width(1.0);
    ras.add_path(stroke);
    agg::render_scanlines(ras, scanLine, rs_box);

    //set text
    agg::gsv_text text_style;
    renderer_solid rs_text(rb);
    //text color in black
    rs_text.color(agg::rgba8(0, 0, 0));
    text_style.size(text_size, text_size);
    text_style.flip(true);
    text_style.text(text.c_str());
    agg::conv_stroke<agg::gsv_text> stroke_text(text_style);
    stroke_text.width(0.8);

    if (y0pos-text_size-2 < 0 || x0pos + text_style.text_width() >= overlay_rgb_width_) {
      return;
    }

    //draw background text
    agg::rounded_rect rec_text(x0pos, y0pos, x0pos + text_style.text_width(), y0pos-text_size-2, 0);
    ras.add_path(rec_text);
    agg::render_scanlines_aa_solid(ras, scanLine, rb,
      agg::rgba8((color&0x00ff0000)>>16, (color&0x0000ff00)>>8, (color&0x000000ff)));
    //draw text
    text_style.start_point(x0pos, y0pos-2);
    ras.add_path(stroke_text);
    agg::render_scanlines(ras, scanLine, rs_text);
  }
  else
  {
    ERR("set_box_with_text() failed\n" );
  }
}

void COverlayRGB::calculate_boundary_text(string text, uint32_t text_size,
                    uint32_t &width, uint32_t &height)
{
  if(!text.empty())
  {
    agg::gsv_text text_style;
    text_style.size(text_size, text_size);
    text_style.flip(true);
    text_style.text(text.c_str());
    width = text_style.text_width();
    //height is double size due to some chareacters: g,q,y ...
    height = 2*text_size;
  }
  else
  {
    width = 0;
    height = 0;
  }
}

void COverlayRGB::set_text(int32_t xpos, int32_t ypos, string text, 
                              uint32_t text_size, uint32_t color, double stroke_size)
{
  if(!text.empty() && buff_rgb_ != NULL)
  {
    pixfmt_type pixf(render_buf_overlay_rgb_);
    base_ren_type rb(pixf);
    agg::scanline_u8 scanLine;
    agg::rasterizer_scanline_aa<> ras;
    //draw text
    agg::gsv_text text_style;
    renderer_solid rs_text(rb);
    rs_text.color(agg::rgba8((color&0x00ff0000)>>16, (color&0x0000ff00)>>8, (color&0x000000ff)));
    text_style.size(text_size, text_size);
    text_style.flip(true);
    text_style.start_point(xpos, ypos+text_size + 1);
    text_style.text(text.c_str());
    agg::conv_stroke<agg::gsv_text> stroke_text(text_style);
    stroke_text.width(stroke_size);
    ras.add_path(stroke_text);
    agg::render_scanlines(ras, scanLine, rs_text);
  }
  else
  {
    ERR("set_text() failed\n" );
  }
}

void COverlayRGB::calculate_boundary_text_with_font(string path_to_ttf, string text, 
                                          uint32_t text_size, uint32_t &w, uint32_t &h)
{
  font_engine_type             feng;
  font_manager_type            fman(feng);
  double x = 0; 
  double y = text_size;
  agg::glyph_rendering gren = agg::glyph_ren_outline;
  if(!path_to_ttf.empty() && !text.empty() && feng.load_font(path_to_ttf.c_str(), 0, gren))
  {
    feng.height(text_size);
    feng.width(text_size);
    feng.hinting(true);
    feng.flip_y(true);
    const char* p = text.c_str();
    double start_x = x;
    while(*p)
    {
      if(*p == '\n')
      {
        x = start_x;
        y -= text_size;
        ++p;
        continue;
      }
      const agg::glyph_cache* glyph = fman.glyph(*p);
      if(glyph != NULL)
      {
        // increment pen position
        x += glyph->advance_x;
        y += glyph->advance_y;
      }
      ++p;
    }
  }
  else
  {
    ERR("calculate_boundary_text_with_font() failed\n" );
  }
  //4/3 is boundary
  h = 4*y/3;
  w = x;
}

void COverlayRGB::set_text_with_font(string path_to_ttf, string text, 
                            double xpos, double ypos, uint32_t height, uint32_t color)
{
  font_engine_type             feng;
  font_manager_type            fman(feng);
  agg::trans_affine mtx;
  agg::conv_curve<font_manager_type::path_adaptor_type> curves(fman.path_adaptor());
  agg::conv_transform<agg::conv_curve<font_manager_type::path_adaptor_type> > trans(curves, mtx);

  pixfmt_type pf(render_buf_overlay_rgb_);
  base_ren_type ren_base(pf);
  renderer_solid ren_solid(ren_base);
  agg::scanline_u8 sl;
  agg::rasterizer_scanline_aa<> ras;
  agg::rgba8 c(agg::rgba8((color&0x00ff0000)>>16, (color&0x0000ff00)>>8, (color&0x000000ff)));
  agg::glyph_rendering gren = agg::glyph_ren_outline;

  if(!path_to_ttf.empty() && !text.empty() && feng.load_font(path_to_ttf.c_str(), 0, gren))
  {
    feng.height(height);
    feng.width(height);
    feng.hinting(true);
    feng.flip_y(true);
    const char* p = text.c_str();
    double start_x = xpos;
    while(*p)
    {
      if(*p == '\n')
      {
        xpos = start_x;
        ypos -= height;
        ++p;
        continue;
      }
      const agg::glyph_cache* glyph = fman.glyph(*p);
      if(glyph != NULL)
      {
        fman.add_kerning(&xpos, &ypos);
        fman.init_embedded_adaptors(glyph, 0, 0);
        if(glyph->data_type == agg::glyph_data_outline)
        {
          double ty = ypos;
          ras.reset();
          mtx.reset();
          mtx *= agg::trans_affine_translation(start_x + xpos, ty);
          ras.add_path(trans);
          
          ren_solid.color(c);
          agg::render_scanlines(ras, sl, ren_solid);
        }

        // increment pen position
        xpos += glyph->advance_x;
        ypos += glyph->advance_y;
      }
      ++p;
    }
  }
  else
  {
    ERR("set_text_with_font() failed\n" );
  }
}

void COverlayRGB::set_box(int32_t x0pos, int32_t y0pos,
                        int32_t x1pos, int32_t y1pos, uint32_t color)
{
  typedef agg::pixfmt_rgb24 pixfmt;
  typedef agg::renderer_base<pixfmt> renderer_base;
  typedef agg::renderer_scanline_aa_solid<renderer_base> renderer_solid;

  if(buff_rgb_ != NULL)
  {
    pixfmt pixf(render_buf_overlay_rgb_);
    renderer_base rb(pixf);

    agg::scanline_u8 scanLine;
    agg::rasterizer_scanline_aa<> ras;
    //draw big box
    renderer_solid rs_box(rb);
    rs_box.color(agg::rgba8((color & 0x00ff0000) >> 16,
          (color & 0x0000ff00) >>  8 , (color & 0x000000ff)));
    agg::rounded_rect rec(x0pos, y0pos, x1pos, y1pos, 0);
    agg::conv_stroke<agg::rounded_rect> stroke(rec);
    stroke.width(2.0);
    ras.add_path(stroke);
    agg::render_scanlines(ras, scanLine, rs_box);
  }
  else
  {
    ERR("set_box() failed\n" );
  }
}

void COverlayRGB::blend_from(COverlayRGB &overlay, double alpha)
{
  typedef agg::rgba8 color_type;
  typedef agg::order_rgba order_type;
  typedef agg::blender_rgb<color_type, order_type> blender_type;
  typedef agg::pixfmt_alpha_blend_rgb<blender_type, rbuf_type, 3> pixfmt_type;
  typedef agg::renderer_base<pixfmt_type> ren_base_type;

  uint32_t w = overlay.get_overlay_width();
  uint32_t h = overlay.get_overlay_height();

  if((screen_width_ >= w) && (screen_height_ >= h) && (buff_rgb_ != NULL))
  {
    pixfmt_type pix_format(overlay.get_ren_buf_ref());
    pixfmt_type pixf(render_buf_overlay_rgb_);
    ren_base_type rb(pixf);
    rb.blend_from(pix_format, 0, 0, 0, unsigned(alpha * 255));
  }
  else
  {
    ERR("blend_from() failed\n" );
  }
}

bool COverlayRGB::load_ppm_img(string filename_wo_ext)
{
    char buf[1024];
    const char* file = filename_wo_ext.c_str();
    strcpy(buf, file);
    int len = strlen(buf);
    if(len < 4 || strcasecmp(buf + len - 4, ".ppm") != 0)
    {
        strcat(buf, ".ppm");
    }

    FILE* fd = fopen(buf, "rb");
    if(fd == 0) return false;

    if((len = fread(buf, 1, 1022, fd)) == 0)
    {
        fclose(fd);
        return false;
    }
    buf[len] = 0;

    if(buf[0] != 'P' && buf[1] != '6')
    {
        fclose(fd);
        return false;
    }

    char* ptr = buf + 2;

    while(*ptr && !isdigit(*ptr)) ptr++;
    if(*ptr == 0)
    {
        fclose(fd);
        return false;
    }

    uint32_t width = atoi(ptr);
    if(width == 0 || width > 4096)
    {
        fclose(fd);
        return false;
    }
    while(*ptr && isdigit(*ptr)) ptr++;
    while(*ptr && !isdigit(*ptr)) ptr++;
    if(*ptr == 0)
    {
        fclose(fd);
        return false;
    }
    uint32_t height = atoi(ptr);
    if(height == 0 || height > 4096)
    {
        fclose(fd);
        return false;
    }
    while(*ptr && isdigit(*ptr)) ptr++;
    while(*ptr && !isdigit(*ptr)) ptr++;
    if(atoi(ptr) != 255)
    {
        fclose(fd);
        return false;
    }
    while(*ptr && isdigit(*ptr)) ptr++;
    if(*ptr == 0)
    {
        fclose(fd);
        return false;
    }
    ptr++;
    fseek(fd, long(ptr - buf), SEEK_SET);
    bool ret = true;
    if(buff_rgb_ != NULL)
    {
      size_t read_len = fread(buff_rgb_, 1, width * height * 3, fd);
      if(read_len < 0)
      {
        ret = false;
      }
    }
    fclose(fd);
    return ret;
}

bool COverlayRGB::save_as_ppm_img(string filename_wo_ext)
{
  char buf[1024];
  const char* file = filename_wo_ext.c_str();
  strcpy(buf, file);
  int len = strlen(buf);
  if(len < 4 || strcasecmp(buf + len - 4, ".ppm") != 0)
  {
    strcat(buf, ".ppm");
  }

  FILE* fd = fopen(buf, "wb");
  if(fd == 0) return false;

  uint32_t w = this->overlay_rgb_width_;
  uint32_t h = this->overlay_rgb_height_;

  fprintf(fd, "P6\n%d %d\n255\n", w, h);

  uint32_t y;
  for(y = 0; y < screen_height_; y++)
  {
    const uint8_t* src = render_buf_overlay_rgb_.row_ptr(y);
    fwrite(src, 1, w * 3, fd);
  }
  fclose(fd);
	return false;
}

void COverlayRGB::print_to_display(uint32_t xpos, uint32_t ypos)
{
  if(buff_rgb_ != NULL)
  {
    uint8_t *fbuf_addr = get_frame_ptr();
    rbuf_type m_rbuf_overlay_bgr;
    uint8_t *m_buff_bgr = new uint8_t[overlay_rgb_width_*overlay_rgb_height_*pixel_size_];
    memset(m_buff_bgr, 0, overlay_rgb_width_*overlay_rgb_height_*pixel_size_);
    m_rbuf_overlay_bgr.attach(m_buff_bgr,
                              overlay_rgb_width_,
                              overlay_rgb_height_,
                              overlay_rgb_width_ * pixel_size_);

    color_conv(&m_rbuf_overlay_bgr, &render_buf_overlay_rgb_, agg::color_conv_rgb24_to_bgr24());
    for (uint32_t i = 0; i < overlay_rgb_height_; i++)
    {
      uint8_t* ptr = m_rbuf_overlay_bgr.row_ptr(i);
      memcpy((void*)(fbuf_addr + xpos * pixel_size_ +
            (ypos + i) * screen_width_ * pixel_size_),
            (void*)(ptr), (overlay_rgb_width_) * pixel_size_);
    }

    delete [] m_buff_bgr;
  }
  else
  {
    ERR("print_to_display() failed\n" );
  }
}

void COverlayRGB::delete_overlay()
{
  if(buff_rgb_ != NULL)
  {
    delete [] buff_rgb_;
    buff_rgb_ = NULL;
  }
}

COverlayRGB::~COverlayRGB()
{
  if(buff_rgb_ != NULL)
  {
    delete [] buff_rgb_;
    buff_rgb_ = NULL;
  }
}

string COverlayRGB::convert_time_to_text(string label, long int time)
{
    std::stringstream ss;
    std::string s;
    if (time >= 0)
    {
      ss.str("");
      ss << std::fixed << std::setprecision(3) << (time / 1000.0) << " ms";
      s = label + ss.str();
    }
    return s;
}

void COverlayRGB::draw_progress_bar(int32_t xpos, int32_t ypos, uint32_t w, uint32_t h,
                  uint32_t color, uint8_t prog_0_to_100) 
{

  typedef agg::pixfmt_rgba32 pixfmt;
  typedef agg::renderer_base<pixfmt> renderer_base;
  typedef agg::renderer_scanline_aa_solid<renderer_base> renderer_solid;

  if((prog_0_to_100 >=0) && (prog_0_to_100 <= 100) && (buff_rgb_ != NULL))
  {
    pixfmt pixf(render_buf_overlay_rgb_);
    renderer_base rb(pixf);

    agg::scanline_u8 scanLine;
    agg::rasterizer_scanline_aa<> ras;

    uint32_t rec_w = (w*prog_0_to_100)/100;
    if((rec_w <= w) && ((xpos+w) <= overlay_rgb_width_) && ((ypos+h) <= overlay_rgb_height_))
    {
      //draw boundary
      renderer_solid rs_box(rb);
      rs_box.color(agg::rgba8((color & 0x00ff0000) >> 16,
            (color & 0x0000ff00) >>  8 , (color & 0x000000ff)));
      agg::rounded_rect rec(xpos, ypos, xpos+w, ypos+h, 0);
      agg::conv_stroke<agg::rounded_rect> stroke(rec);
      stroke.width(1.0);
      ras.add_path(stroke);
      agg::render_scanlines(ras, scanLine, rs_box);
      //draw progress
      agg::rounded_rect rec_bar(xpos, ypos, xpos + rec_w, h, 0);
      ras.add_path(rec_bar);
      agg::render_scanlines_aa_solid(ras, scanLine, rb,
        agg::rgba8((color&0x00ff0000)>>16, (color&0x0000ff00)>>8, (color&0x000000ff)));
    }
    else
    {
      ERR("draw_progress_bar() failed\n" );
    }
  }
  else
  {
    ERR("draw_progress_bar() failed\n" );
  }
}

void COverlayRGB::capture_screen(std::string filename, 
                              uint32_t screen_width, uint32_t screen_height) {
  FILE *fout = fopen(filename.c_str(), "wb");
  if (!fout) {
    ERR("Could not open %s for writing\n", filename.c_str());
    return;
  }
  if (fwrite(get_frame_ptr(), 3, screen_width * screen_height, fout) != 
                                                        screen_width * screen_height) {
    ERR("Incomplete write to %s\n", filename.c_str());
  }
  fclose(fout);
}

void COverlayRGB::set_mouse_cursor(uint32_t m_color)
{

  if( buff_rgb_ != NULL)
  {
    pixfmt_type pixf( render_buf_overlay_rgb_);
    base_ren_type ren(pixf);
    agg::scanline_u8 sl;
    agg::rasterizer_scanline_aa<> ras;

    ras.gamma(agg::gamma_power( 1));
    renderer_solid ren_sl(ren);
    agg::rgba8 color = agg::rgba8(( m_color&0x00ff0000)>>16,
        (m_color&0x0000ff00)>>8, (m_color&0x000000ff), (m_color&0xff000000)>>24);

    ras.reset();
        ras.move_to_d( 0, 0);
        ras.line_to_d( 0, 9);
        ras.line_to_d( 3, 6);
        ras.line_to_d( 6, 6);
        ras.close_polygon();
        ren_sl.color(color);
    agg::render_scanlines(ras, sl, ren_sl);

    ras.gamma(agg::gamma_none());

    agg::path_storage ps;
    agg::conv_stroke<agg::path_storage> pg(ps);
    pg.width( 1.2);

    ps.remove_all();
    ps.move_to( 2.3, 6);
    ps.line_to( 4, 9);
    ras.add_path(pg);
    agg::render_scanlines_aa_solid(ras, sl, ren, color);

  }
  else
  {
    fprintf(stderr, "set_mouse_cursor() failed\n" );
  }
}

void COverlayRGB::set_background_to_color(uint32_t m_color){
  if( buff_rgb_ != NULL){
    pixfmt_type pixf( render_buf_overlay_rgb_);
    base_ren_type ren(pixf);
    agg::rgba8 color = agg::rgba8(( m_color&0x00ff0000)>>16,
          (m_color&0x0000ff00)>>8, (m_color&0x000000ff), (m_color&0xff000000)>>24);
    ren.clear( color);
  }
  else{
    ERR( "set_background_to_color() failed\n" );
  }
}

void COverlayRGB::draw_ctrlBox_with_fonts(int32_t x0pos, int32_t y0pos, int32_t x1pos, int32_t y1pos,
    uint32_t box_color, uint32_t outline_color,
    string path_to_ttf, string txt, uint32_t txt_size, uint32_t text_color, double c_radius)
{
  if( buff_rgb_ != NULL) {

    pixfmt_type pixf( render_buf_overlay_rgb_);
    agg::scanline_u8 scanLine;
    agg::rasterizer_scanline_aa<> ras;

    base_ren_type rb(pixf);
    renderer_solid rs_box(rb);

    rs_box.color(agg::rgba8((box_color & 0x00ff0000) >> 16,
          (box_color & 0x0000ff00) >>  8, (box_color & 0x000000ff), (box_color & 0xff000000) >> 24));
    agg::rounded_rect rec(x0pos, y0pos, x1pos, y1pos, c_radius);
    rec.normalize_radius();
        ras.add_path( rec);
    agg::render_scanlines(ras, scanLine, rs_box);

    rs_box.color(agg::rgba8((outline_color & 0x00ff0000) >> 16,
              (outline_color & 0x0000ff00) >>  8 , (outline_color & 0x000000ff), (outline_color & 0xff000000) >> 24));
    agg::conv_stroke<agg::rounded_rect> stroke(rec);
    stroke.width( 2.0);
    ras.add_path(stroke);
    agg::render_scanlines(ras, scanLine, rs_box);

    if( txt != ""){
      font_engine_type             feng;
      font_manager_type            fman(feng);
      agg::trans_affine mtx;
      agg::conv_curve<font_manager_type::path_adaptor_type> curves(fman.path_adaptor());
      agg::conv_transform<agg::conv_curve<font_manager_type::path_adaptor_type> > trans(curves, mtx);
      agg::glyph_rendering gren = agg::glyph_ren_outline;

      uint32_t w = 0;
      uint32_t h = 0;
      // Text size that input to funtions in font engine are required in 100*inch unit.
      // That is why we need to change to pixel
      double txt_height = double( txt_size)*100/64; // 64DPI
      COverlayRGB::calculate_boundary_text_with_font( path_to_ttf, txt, txt_height, w, h);

      rs_box.color(agg::rgba8((text_color & 0x00ff0000) >> 16,
                    (text_color & 0x0000ff00) >>  8 , (text_color & 0x000000ff), (text_color & 0xff000000) >> 24));

      // Render each by each letter in the string
      if(!path_to_ttf.empty() && !txt.empty() && feng.load_font(path_to_ttf.c_str(), 0, gren)){

        feng.height( txt_height);
        feng.width( txt_height);
        feng.hinting(true);
        feng.flip_y(true);
        const char* p = txt.c_str();
        double x = 0;
        double y = 0;
        double start_x = x0pos + (x1pos - x0pos)/2 - w/2;
        double start_y = y0pos + (y1pos - y0pos)/2 + txt_size/2 + txt_size/15;
        while(*p)
        {
          // This block is for further development when new line is needed in the text
          /*if(*p == '\n')
          {
          start_y += txt_size*2;
          ++p;
          x = 0;
          y = 0;
          continue;
          }*/
          const agg::glyph_cache* glyph = fman.glyph(*p);
          if(glyph != NULL)
          {
          fman.add_kerning(&x, &y);
          fman.init_embedded_adaptors(glyph, 0, 0);
          if(glyph->data_type == agg::glyph_data_outline)
          {
            ras.reset();
            mtx.reset();
            mtx *= agg::trans_affine_translation( start_x + x, start_y + y);
            ras.add_path(trans);

            agg::render_scanlines(ras, scanLine, rs_box);
          }
          // increment pen position
          x += glyph->advance_x;
          y += glyph->advance_y;
          }
          ++p;
        }
      }
      else{
        ERR("set_text_with_font() failed\n" );
      }
    }
  }
  else{
    fprintf(stderr, "draw_ctrlBox() failed\n" );
  }
}

};  // end of namespace util
};  // end of namespace dmp
