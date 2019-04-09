[1mdiff --git a/src/seed_predict.cpp b/src/seed_predict.cpp[m
[1mindex 3fad809..cb84e3c 100644[m
[1m--- a/src/seed_predict.cpp[m
[1m+++ b/src/seed_predict.cpp[m
[36m@@ -71,6 +71,167 @@[m [mstd::vector<std::string> catstr_vec(categories, categories + 1000);[m
 std::vector<std::string> seed_strings = { "NG","OK" };[m
 std::vector<uint32_t> seed_colors = {RED, BLUE};[m
 [m
[32m+[m
[32m+[m[32mvoid preproc_image_gray(uint32_t *__restrict imgView, __fp16 *__restrict imgProc,[m
[32m+[m[32m                   int img_width, int img_height, float r_offs, float g_offs,[m
[32m+[m[32m                   float b_offs, float sf, bool transpose, bool is_bgr) {[m
[32m+[m[32m  int i = 0, j = 0;[m
[32m+[m[32m  for (int y = 0; y < img_height; y++) {[m
[32m+[m[32m    if (transpose) j = 1 * y;[m
[32m+[m[32m    for (int x = 0; x < img_width; x++) {[m
[32m+[m[32m      uint32_t rgba = imgView[i];[m
[32m+[m[32m      i++;[m
[32m+[m[32m      unsigned char r = (rgba >> 24);[m
[32m+[m[32m      unsigned char g = (rgba >> 16) & 0xFF;[m
[32m+[m[32m      unsigned char b = (rgba >> 8) & 0xFF;[m
[32m+[m[32m      imgProc[j] = (__fp16)(sf * (b_offs + (r+g+b)/3));[m
[32m+[m[32m      j++;[m
[32m+[m[32m      }[m
[32m+[m[32m      if (transpose) j += 1 * (img_height - 1);[m
[32m+[m[32m    }[m
[32m+[m[32m  }[m
[32m+[m
[32m+[m
[32m+[m[32mvoid preproc_image_gray(uint32_t *__restrict imgView, __fp16 *__restrict imgProc,[m
[32m+[m[32m                   int inimg_width, int inimg_height, int outimg_width,[m
[32m+[m[32m                   int outimg_height, float r_offs, float g_offs, float b_offs,[m
[32m+[m[32m                   float sf, bool transpose, bool is_bgr) {[m
[32m+[m[32m  uint32_t *imgIn = imgView, *imgOut = nullptr;[m
[32m+[m
[32m+[m[32m  while (inimg_width >= outimg_width * 2 && inimg_height >= outimg_height * 2) {[m
[32m+[m[32m    int nw = inimg_width / 2;[m
[32m+[m[32m    int nh = inimg_height / 2;[m
[32m+[m[32m    imgOut = new uint32_t[nw * nh];[m
[32m+[m[32m    for (int y = 0; y < nh; y++) {[m
[32m+[m[32m      for (int x = 0; x < nw; x++) {[m
[32m+[m[32m        uint32_t p, pr = 0, pg = 0, pb = 0;[m
[32m+[m[32m        p = imgIn[(y * 2 + 0) * inimg_width + (x * 2 + 0)];[m
[32m+[m[32m        pr += (p >> 24);[m
[32m+[m[32m        pg += (p >> 16) & 0xFF;[m
[32m+[m[32m        pb += (p >> 8) & 0xFF;[m
[32m+[m[32m        p = imgIn[(y * 2 + 0) * inimg_width + (x * 2 + 1)];[m
[32m+[m[32m        pr += (p >> 24);[m
[32m+[m[32m        pg += (p >> 16) & 0xFF;[m
[32m+[m[32m        pb += (p >> 8) & 0xFF;[m
[32m+[m[32m        p = imgIn[(y * 2 + 1) * inimg_width + (x * 2 + 0)];[m
[32m+[m[32m        pr += (p >> 24);[m
[32m+[m[32m        pg += (p >> 16) & 0xFF;[m
[32m+[m[32m        pb += (p >> 8) & 0xFF;[m
[32m+[m[32m        p = imgIn[(y * 2 + 1) * inimg_width + (x * 2 + 1)];[m
[32m+[m[32m        pr += (p >> 24);[m
[32m+[m[32m        pg += (p >> 16) & 0xFF;[m
[32m+[m[32m        pb += (p >> 8) & 0xFF;[m
[32m+[m[32m        pr /= 4;[m
[32m+[m[32m        pg /= 4;[m
[32m+[m[32m        pb /= 4;[m
[32m+[m[32m        imgOut[y * nw + x] = (pr << 24) | (pg << 16) | (pb << 8);[m
[32m+[m[32m      }[m
[32m+[m[32m    }[m
[32m+[m[32m    inimg_width = nw;[m
[32m+[m[32m    inimg_height = nh;[m
[32m+[m[32m    if (imgIn != imgView) delete[] imgIn;[m
[32m+[m[32m    imgIn = imgOut;[m
[32m+[m[32m  }[m
[32m+[m
[32m+[m[32m  while (inimg_width >= outimg_width * 2) {[m
[32m+[m[32m    int nw = inimg_width / 2;[m
[32m+[m[32m    int nh = inimg_height;[m
[32m+[m[32m    imgOut = new uint32_t[nw * nh];[m
[32m+[m[32m    for (int y = 0; y < nh; y++) {[m
[32m+[m[32m      for (int x = 0; x < nw; x++) {[m
[32m+[m[32m        uint32_t p, pr = 0, pg = 0, pb = 0;[m
[32m+[m[32m        p = imgIn[y * inimg_width + (x * 2 + 0)];[m
[32m+[m[32m        pr += (p >> 24);[m
[32m+[m[32m        pg += (p >> 16) & 0xFF;[m
[32m+[m[32m        pb += (p >> 8) & 0xFF;[m
[32m+[m[32m        p = imgIn[y * inimg_width + (x * 2 + 1)];[m
[32m+[m[32m        pr += (p >> 24);[m
[32m+[m[32m        pg += (p >> 16) & 0xFF;[m
[32m+[m[32m        pb += (p >> 8) & 0xFF;[m
[32m+[m[32m        pr /= 2;[m
[32m+[m[32m        pg /= 2;[m
[32m+[m[32m        pb /= 2;[m
[32m+[m[32m        imgOut[y * nw + x] = (pr << 24) | (pg << 16) | (pb << 8);[m
[32m+[m[32m      }[m
[32m+[m[32m    }[m
[32m+[m[32m    inimg_width = nw;[m
[32m+[m[32m    if (imgIn != imgView) delete[] imgIn;[m
[32m+[m[32m    imgIn = imgOut;[m
[32m+[m[32m  }[m
[32m+[m
[32m+[m[32m  while (inimg_height >= outimg_height * 2) {[m
[32m+[m[32m    int nw = inimg_width;[m
[32m+[m[32m    int nh = inimg_height / 2;[m
[32m+[m[32m    imgOut = new uint32_t[nw * nh];[m
[32m+[m[32m    for (int y = 0; y < nh; y++) {[m
[32m+[m[32m      for (int x = 0; x < nw; x++) {[m
[32m+[m[32m        uint32_t p, pr = 0, pg = 0, pb = 0;[m
[32m+[m[32m        p = imgIn[(y * 2 + 0) * inimg_width + x];[m
[32m+[m[32m        pr += (p >> 24);[m
[32m+[m[32m        pg += (p >> 16) & 0xFF;[m
[32m+[m[32m        pb += (p >> 8) & 0xFF;[m
[32m+[m[32m        p = imgIn[(y * 2 + 1) * inimg_width + x];[m
[32m+[m[32m        pr += (p >> 24);[m
[32m+[m[32m        pg += (p >> 16) & 0xFF;[m
[32m+[m[32m        pb += (p >> 8) & 0xFF;[m
[32m+[m[32m        pr /= 2;[m
[32m+[m[32m        pg /= 2;[m
[32m+[m[32m        pb /= 2;[m
[32m+[m[32m        imgOut[y * nw + x] = (pr << 24) | (pg << 16) | (pb << 8);[m
[32m+[m[32m      }[m
[32m+[m[32m    }[m
[32m+[m[32m    inimg_height = nh;[m
[32m+[m[32m    if (imgIn != imgView) delete[] imgIn;[m
[32m+[m[32m    imgIn = imgOut;[m
[32m+[m[32m  }[m
[32m+[m
[32m+[m[32m  if (inimg_width != outimg_width || inimg_height != outimg_height) {[m
[32m+[m[32m    int nw = outimg_width;[m
[32m+[m[32m    int nh = outimg_height;[m
[32m+[m[32m    float sx = float(inimg_width) / float(outimg_width);[m
[32m+[m[32m    float sy = float(inimg_height) / float(outimg_height);[m
[32m+[m[32m    imgOut = new uint32_t[nw * nh];[m
[32m+[m[32m    for (int y = 0; y < nh; y++) {[m
[32m+[m[32m      float ty = y * sy;[m
[32m+[m[32m      int iy = int(ty);[m
[32m+[m[32m      float ry = ty - iy;[m
[32m+[m[32m      for (int x = 0; x < nw; x++) {[m
[32m+[m[32m        float tx = x * sx;[m
[32m+[m[32m        int ix = int(tx);[m
[32m+[m[32m        float rx = tx - ix;[m
[32m+[m[32m        float r = 0.f, g = 0.f, b = 0.f;[m
[32m+[m[32m        uint32_t p;[m
[32m+[m[32m        p = imgIn[(iy + 0) * inimg_width + (ix + 0)];[m
[32m+[m[32m        r += (p >> 24) * rx * ry;[m
[32m+[m[32m        g += ((p >> 16) & 0xFF) * rx * ry;[m
[32m+[m[32m        b += ((p >> 8) & 0xFF) * rx * ry;[m
[32m+[m[32m        p = imgIn[(iy + 0) * inimg_width + (ix + 1)];[m
[32m+[m[32m        r += (p >> 24) * (1.f - rx) * ry;[m
[32m+[m[32m        g += ((p >> 16) & 0xFF) * (1.f - rx) * ry;[m
[32m+[m[32m        b += ((p >> 8) & 0xFF) * (1.f - rx) * ry;[m
[32m+[m[32m        p = imgIn[(iy + 1) * inimg_width + (ix + 0)];[m
[32m+[m[32m        r += (p >> 24) * rx * (1.f - ry);[m
[32m+[m[32m        g += ((p >> 16) & 0xFF) * rx * (1.f - ry);[m
[32m+[m[32m        b += ((p >> 8) & 0xFF) * rx * (1.f - ry);[m
[32m+[m[32m        p = imgIn[(iy + 1) * inimg_width + (ix + 1)];[m
[32m+[m[32m        r += (p >> 24) * (1.f - rx) * (1.f - ry);[m
[32m+[m[32m        g += ((p >> 16) & 0xFF) * (1.f - rx) * (1.f - ry);[m
[32m+[m[32m        b += ((p >> 8) & 0xFF) * (1.f - rx) * (1.f - ry);[m
[32m+[m[32m        unsigned char pr = static_cast<unsigned char>(r);[m
[32m+[m[32m        unsigned char pg = static_cast<unsigned char>(g);[m
[32m+[m[32m        unsigned char pb = static_cast<unsigned char>(b);[m
[32m+[m[32m        imgOut[y * nw + x] = (pr << 24) | (pg << 16) | (pb << 8);[m
[32m+[m[32m      }[m
[32m+[m[32m    }[m
[32m+[m[32m    inimg_width = nw;[m
[32m+[m[32m    inimg_height = nh;[m
[32m+[m[32m    if (imgIn != imgView) delete[] imgIn;[m
[32m+[m[32m    imgIn = imgOut;[m
[32m+[m[32m  }[m
[32m+[m[32m  preproc_image_gray(imgIn, imgProc, outimg_width, outimg_height, r_offs, g_offs,[m
[32m+[m[32m                b_offs, sf, transpose, is_bgr);[m
[32m+[m[32m  if (imgIn != imgView) delete[] imgIn;[m
[32m+[m[32m}[m
 int main(int argc, char **argv) {[m
     cout << "start" << endl;[m
 [m
[36m@@ -109,7 +270,7 @@[m [mint main(int argc, char **argv) {[m
     // Buffer for decoded image data[m
     uint32_t imgView[IMAGE_W * IMAGE_H];[m
     // Buffer for pre-processed image data[m
[31m-    __fp16 imgProc[PIMAGE_W * PIMAGE_H * 3];[m
[32m+[m[32m    __fp16 imgProc[PIMAGE_W * PIMAGE_H * 1];[m
     // buffer for result[m
 [m
 [m
[36m@@ -142,7 +303,6 @@[m [mint main(int argc, char **argv) {[m
     while (exit_code == -1) {[m
         // If not pause, get next image from WebCam[m
         if (!pause) {[m
[31m-            /* cap.read(frame); */[m
             if (capture_cam(imgView, CIMAGE_W, CIMAGE_H, 0, 0, CIMAGE_W, CIMAGE_H))[m
             {[m
                 cout << "Camera error" << endl;[m
[36m@@ -151,11 +311,11 @@[m [mint main(int argc, char **argv) {[m
             // 推測にまわすデータを作成[m
             cam_overlay.convert_to_overlay_pixel_format(imgView, CIMAGE_W*CIMAGE_H);[m
             // Pre-process the image data[m
[31m-            preproc_image(imgView, imgProc, IMAGE_W, IMAGE_H, PIMAGE_W, PIMAGE_H, 0.0, 0.0, 0.0, 1.0 / 255.0, true, false);[m
[32m+[m[32m            preproc_image_gray(imgView, imgProc, IMAGE_W, IMAGE_H, PIMAGE_W, PIMAGE_H, 0.0, 0.0, 0.0, 1.0 / 255.0, true, false);[m
         }[m
 [m
         // Run network in HW[m
[31m-        memcpy(network.get_network_input_addr_cpu(), imgProc, PIMAGE_W * PIMAGE_H * 6);[m
[32m+[m[32m        memcpy(network.get_network_input_addr_cpu(), imgProc, PIMAGE_W * PIMAGE_H * 2);[m
         network.RunNetwork();[m
 [m
         // Handle output from HW[m
[36m@@ -175,7 +335,7 @@[m [mint main(int argc, char **argv) {[m
         COverlayRGB predict_text(SCREEN_W, SCREEN_H);[m
         predict_text.alloc_mem_overlay(w, h);[m
 [m
[31m-        int x = SCREEN_W/2;[m
[32m+[m[32m        int x = SCREEN_W / 2;[m
         int y = SCREEN_H * 0.75;[m
 [m
         //clear previous text[m
