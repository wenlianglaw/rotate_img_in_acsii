#define cimg_display 0
#include "CImg.h"

#include <cstdint>
#include <iostream>
#include <string>

#include "world.h"

using namespace cimg_library;

class ImageLoader {
public:
  void LoadImageIntoWorld(const std::string &filepath, World &world) {
    try {
      CImg<unsigned char> img(filepath.c_str());

      int img_w = img.width();
      int img_h = img.height();
      int channels = img.spectrum();

      std::cout << "Loaded image: " << img_w << "x" << img_h << " (" << channels
                << " channels)\n";

      int w = world.GetWidth();
      int h = world.GetHeight();
      int z_center = world.GetDepth() / 2;

      for (int y = 0; y < h; ++y) {
        for (int x = 0; x < w; ++x) {
          // Nearest neighbor sampling
          int img_x = (x * img_w) / w;
          int img_y = (y * img_h) / h;

          uint8_t brightness = 0;
          if (channels >= 3) {
            int r = img(img_x, img_y, 0, 0);
            int g = img(img_x, img_y, 0, 1);
            int b = img(img_x, img_y, 0, 2);
            brightness = (r + g + b) / 3;

            if (channels == 4 && img(img_x, img_y, 0, 3) < 128) {
              brightness = 0; // Ignore transparent pixels
            }
          } else if (channels == 1 || channels == 2) {
            brightness = img(img_x, img_y, 0, 0);
            if (channels == 2 && img(img_x, img_y, 0, 1) < 128) {
              brightness = 0;
            }
          }

          // Give the image 3D "thickness" by plotting it across multiple
          // Z-planes
          if (brightness > 0) {
            for (int dz = -2; dz <= 2; ++dz) {
              if (world.ValidRange(x, y, z_center + dz)) {
                world.at(x, y, z_center + dz) = brightness;
              }
            }
          }
        }
      }
    } catch (CImgException &e) {
      std::cerr << "Failed to load image: " << filepath << "\n";
      std::cerr << "Reason: " << e.what() << "\n";
    }
  }
};
