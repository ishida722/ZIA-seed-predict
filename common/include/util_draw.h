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

#ifndef __DRAW_UTIL
#define __DRAW_UTIL

#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <unistd.h>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <utility>
#include <algorithm>
#include <iostream>
#include <string>

#include <cstring>
#include <math.h>
#include "agg_rasterizer_scanline_aa.h"
#include "agg_conv_transform.h"
#include "agg_span_image_filter_rgb.h"
#include "agg_span_image_filter_rgba.h"
#include "agg_pixfmt_rgba.h"
#include "agg_renderer_scanline.h"
#include "agg_scanline_p.h"
#include "agg_scanline_u.h"
#include "agg_rounded_rect.h"
#include "util/agg_color_conv_rgb8.h"
#include "util/agg_color_conv.h"
#include "agg_basics.h"
#include "agg_rendering_buffer.h"
#include "agg_pixfmt_rgb.h"
#include "agg_conv_stroke.h"
#include "agg_gsv_text.h"
#include "agg_embedded_raster_fonts.h"
#include "agg_glyph_raster_bin.h"
#include "agg_renderer_raster_text.h"
#include "agg_scanline_bin.h"
#include "agg_renderer_primitives.h"
#include "agg_conv_curve.h"
#include "agg_conv_contour.h"
#include "agg_font_freetype.h"
#include "agg_gamma_lut.h"
#include "ctrl/agg_slider_ctrl.h"
#include "ctrl/agg_cbox_ctrl.h"
#include "ctrl/agg_rbox_ctrl.h"

using namespace std;

namespace dmp {
namespace util {

typedef agg::rendering_buffer rbuf_type;
typedef agg::pixfmt_rgb24 pixfmt_type;
typedef agg::renderer_base<pixfmt_type> base_ren_type;
typedef agg::renderer_scanline_aa_solid<base_ren_type> renderer_solid;
typedef agg::font_engine_freetype_int32 font_engine_type;
typedef agg::font_cache_manager<font_engine_type> font_manager_type;

class COverlayRGB
{
  private:
    // raw buffer holds overlay
    uint8_t                      *buff_rgb_ = NULL;
    // width of overlay
    uint32_t                     overlay_rgb_width_;
    // height of overlay
    uint32_t                     overlay_rgb_height_;
    // width of screen
    uint32_t                     screen_width_;
    // height of screen
    uint32_t                     screen_height_;
    // bytes of a pixel
    uint32_t                     pixel_size_;
    // render buffer holds overlay
    rbuf_type                         render_buf_overlay_rgb_;

    public:
    /*! Utility constructor to create a overlay with rgb24 color format 
    * then using alloc_mem_overlay() to allocate memory for overlay
    * @param[in] screen_width Width of the screen
    * @param[in] screen_height Height of the screen
    */
    COverlayRGB(uint32_t screen_width, uint32_t screen_height);

    /*! Utility function to allocate memory for overlay
    * @param[in] overlay_width Width of the overlay
    * @param[in] overlay_height Height of the overlay
    */
    void alloc_mem_overlay(uint32_t overlay_width, uint32_t overlay_height);

    /*! Utility function to get the width of overlay
    * @return width of overlay
    */
    uint32_t get_overlay_width(void)
    {
      return overlay_rgb_width_;
    }

    /*! Utility function to get the height of overlay
    * @return height of overlay
    */
    uint32_t get_overlay_height(void)
    {
      return overlay_rgb_height_;
    }

    /*! Utility function to get overlay buffer reference
     * which is used to fill data to overlay buffer
    * @return reference to overlay buffer
    */
    uint8_t* get_overlay_buf_ref(void);

    /*! Utility function to convert from pixel in integer format to overlay rgba32 pixel format
     * and both must have same (width x height)
    * @param[in] imgview Pointer to buffer of type integer
    * @param[in] size_of_imgview Size of the buffer
    * @return operation is successful or not
    */
    bool convert_to_overlay_pixel_format(uint32_t *imgview, uint32_t size_of_imgview);

    /*! Utility function to get rendering buffer reference
     * which is used to fill data to overlay buffer
    * @return reference to rendering buffer
    */
    rbuf_type& get_ren_buf_ref(void);

    /*! Utility function to get reference to a row of overlay
     * which is used to fill data to overlay buffer
    * @param[in] row Row need to get reference
    * @return reference to a row of overlay
    */
    uint8_t* get_overlay_row_ptr_ref(uint32_t row);


    /*! Utility function to set pixel at a specific position with rgba value
    * @param[in] xpos x position
    * @param[in] ypos y position
    * @param[in] red  red value
    * @param[in] green green value
    * @param[in] blue blue value
    * @return operation is successful or not
    */
    bool set_pixel(uint32_t xpos, uint32_t ypos,
                uint8_t red, uint8_t green, uint8_t blue);

    /*! Utility function to copy an area of another overlay - destination overlay fits in copied area
    * @param[in] source overlay to be copied
    * @param[in] xpos x position that start copying
    * @param[in] ypos  y position that start copying
    * @return operation is successful or not
    */
    bool copy_overlay(COverlayRGB &src_overlay, uint32_t xpos, uint32_t ypos);

    /*! Utility function to draw box with text, color is argb format
    * @param[in] x0pos top-left x position of box in overlay
    * @param[in] y0pos top-left y position of box in overlay
    * @param[in] x1pos bottom-right x position of box in overlay
    * @param[in] y1pos bottom-right y position of box in overlay
    * @param[in] color color of box
    * @param[in] text text content
    */
    void set_box_with_text(int32_t x0pos, int32_t y0pos, int32_t x1pos,
                                  int32_t y1pos, uint32_t color, string text);

    /*! Utility function to draw text, color is argb format.
     * This function depends on calculate_boundary_text()
    * @param[in] x0pos top-left x position of text in overlay
    * @param[in] y0pos top-left y position of text in overlay
    * @param[in] text text content
    * @param[in] text_size size of text
    * @param[in] color color of text
    */
    void set_text(int32_t xpos, int32_t ypos, string text, 
                        uint32_t text_size, uint32_t color, double stroke_size = 1.5);

    /*! Utility function to draw text when using font, color is argb format.
     * This function depends on calculate_boundary_text_with_font()
    * @param[in] path_to_ttf path to font file 
    * @param[in] xpos top-left x position of text in overlay
    * @param[in] ypos top-left y position of text in overlay
    * @param[in] text text content
    * @param[in] text_size size of text
    * @param[in] color color of text
    * @param[in] italic set italic text
    */
    void set_text_with_font(string path_to_ttf, string text, 
                          double xpos, double ypos, uint32_t text_size, uint32_t color);

    /*! Utility function to draw box without text, color is argb format
    * @param[in] x0pos top-left x position of box in overlay
    * @param[in] y0pos top-left y position of box in overlay
    * @param[in] x1pos bottom-right x position of box in overlay
    * @param[in] y1pos bottom-right y position of box in overlay
    * @param[in] color color of box
    */
    void set_box(int32_t x0pos, int32_t y0pos,
                    int32_t x1pos, int32_t y1pos, uint32_t color);

    /*! Utility function to blend 2 overlays with same size
    * @param[in] overlay overlay to blend
    * @param[in] alpha alpha value
    */
    void blend_from(COverlayRGB &overlay, double alpha=0.5);

    /*! Utility function to load ppm file and file name without extension
    * @param[in] filename_wo_ext file name without extension
    * @return operation is successful or not
    */
    bool load_ppm_img(string filename_wo_ext);

    /*! Utility function to save as ppm format and file name without extension
    * @param[in] filename_wo_ext file name without extension
    * @return operation is successful or not
    */
    bool save_as_ppm_img(string filename_wo_ext);

    /*! Utility function for drawing a progress bar
    * @param[in] xpos x-position of progress bar in overlay
    * @param[in] ypos y-position of progress bar in overlay
    * @param[in] w width of progress bar in overlay
    * @param[in] h height of progress bar in overlay
    * @param[in] color color of progress bar
    * @param[in] prog_0_to_100 progress from 0 to 100
    */
    void draw_progress_bar(int32_t xpos, int32_t ypos, uint32_t w, uint32_t h,
            uint32_t color, uint8_t prog_0_to_100) ;

    /*! Utility function to print overlay to display
    * @param[in] xpos top-left x position of overlay in display
    * @param[in] ypos top-left y position of overlay in display
    */
    void print_to_display(uint32_t xpos, uint32_t ypos);

    /*! Utility function to delete a overlay
    */
    void delete_overlay();

        /*! destructor to delete a overlay
    */
    virtual ~COverlayRGB();

    /*! Utility function to convert time to text
    * @param[in] label label of time
    * @param[in] time value to convert
    */
    static string convert_time_to_text(string label, long int time);

    /*! Utility function to calculate boundary of text
    * @param[in] text text content
    * @param[in] text_size size of text
    * @param[out] width width of boundary
    * @param[out] height height of boundary
    */
    static void calculate_boundary_text(string text, uint32_t text_size,
                      uint32_t &width, uint32_t &height);

    /*! Utility function to calculate boundary of text whe using font
    * @param[in] path_to_ttf path to font file
    * @param[in] text text content
    * @param[in] text_size size of text
    * @param[out] width width of boundary
    * @param[out] height height of boundary
    */
    static void calculate_boundary_text_with_font(string path_to_ttf, string text, 
                                    uint32_t text_size, uint32_t &w, uint32_t &h);                  

    /*! Utility function to capture the screen
    */
    static void capture_screen(std::string filename,
            uint32_t screen_width, uint32_t screen_height);

	/*! Utility function to draw mouse cursor with a specific color onto screen
	* @param[in] m_color color of the mouse cursor
	*/
    void set_mouse_cursor(uint32_t m_color);

	/*! Utility function to set color to the buffer
	* @param[in] m_color color of background
	*/
    void set_background_to_color(uint32_t m_color);

	/*! Utility function to draw a control box with text in the case of using specific font
	* @param[in] x0pos top-left x position of the box in overlay
	* @param[in] y0pos top-left y position of the box in overlay
	* @param[in] x1pos bottom-right x position of the box in overlay
	* @param[in] y1pos bottom-right y position of the box in overlay
	* @param[in] box_color background color of the box
	* @param[in] outline_color color for the outline of the box
	* @param[in] path_to_ttf path of font in string
	* @param[in] txt input text that display inside the box, if no text, put ""
	* @param[in] txt_size size of the text
	* @param[in] text_color color of the text
	* @param[in] c_radius radius of square corners of the box
	*/
    void draw_ctrlBox_with_fonts(int32_t x0pos, int32_t y0pos, int32_t x1pos, int32_t y1pos,
        uint32_t box_color, uint32_t outline_color,
        string path_to_ttf, string txt, uint32_t txt_size, uint32_t text_color, double c_radius);

};

/// @brief Initializes the framebuffer (/dev/fb0).
/// @details If the /dev/tty0 was in text mode, it will be switched to graphics mode
///          and switched back on shutdown(), SIGINT, SIGQUIT or SIGTERM.
bool init_fb();

/// @brief Releases allocated resources if any (closes framebuffer).
void shutdown();

/// @brief Swaps the framebuffer.
bool swap_buffer();

/// @brief Returns framebuffer width in pixels.
uint32_t get_screen_width();

/// @brief Returns framebuffer height in pixels.
uint32_t get_screen_height();

void print_result(const std::vector<std::string>& catstr_vec,
                  int x, int y, const std::vector<std::pair<float, int> >& f, COverlayRGB &bg_overlay);

std::vector<std::pair<float, int> > catrank(float* softmax, int count=1000);

};  // end of namespace util
};  // end of namespace dmp

#endif  //__DRAW_UTIL
