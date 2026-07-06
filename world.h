#pragma once

#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstdint>
#include <iostream>
#include <string>
#include <vector>

#include "char_set.h"

char GetCharForBrightness(uint8_t brightness);

class World {
private:
  int width, height, depth;

  // Display configuration
  int chars_per_voxel_x;
  int chars_per_voxel_y;

  // A 1D vector acting as a 3D array
  std::vector<uint8_t> data;

public:
  // Updated constructor to accept character scaling with defaults
  World(int w, int h, int d, int char_x = 2, int char_y = 1)
      : width(w), height(h), depth(d), chars_per_voxel_x(char_x),
        chars_per_voxel_y(char_y), data(w * h * d, 0) {}

  int GetWidth() const { return width; }
  int GetHeight() const { return height; }
  int GetDepth() const { return depth; }

  bool ValidRange(int x, int y, int z) const {
    return x >= 0 && y >= 0 && z >= 0 && x < width && y < height && z < depth;
  }

  uint8_t &at(int x, int y, int z) {
    return data[z * width * height + y * width + x];
  }

  uint8_t at(int x, int y, int z) const {
    return data[z * width * height + y * width + x];
  }

  void Clear() { std::fill(data.begin(), data.end(), 0); }

  void RotateFrom(const World &original, float theta) {
    Clear();

    float cx = width / 2.0f;
    float cy = height / 2.0f;
    float cz = depth / 2.0f;

    float c = std::cos(theta);
    float s = std::sin(theta);

    for (int x = 0; x < width; ++x) {
      for (int y = 0; y < height; ++y) {
        for (int z = 0; z < depth; ++z) {

          float dx = x - cx;
          float dy = y - cy;
          float dz = z - cz;

          int ox = std::round(dx * c + dz * s + cx);
          int oy = std::round(dy + cy);
          int oz = std::round(-dx * s + dz * c + cz);

          if (original.ValidRange(ox, oy, oz)) {
            uint8_t val = original.at(ox, oy, oz);
            if (val > 0) {
              at(x, y, z) = val;
            }
          }
        }
      }
    }
  }

  uint8_t ProjectZ(int x, int y) const {
    uint8_t max_brightness = 0;
    for (int z = 0; z < depth; ++z) {
      uint8_t val = at(x, y, z);
      if (val > max_brightness) {
        max_brightness = val;
      }
    }
    return max_brightness;
  }

  void Display() const {
    std::string frame_buffer;
    // Calculate total buffer size to prevent reallocation mid-draw
    int row_width = (width * chars_per_voxel_x) + 1; // +1 for newline
    int total_rows = height * chars_per_voxel_y;
    frame_buffer.reserve(row_width * total_rows);

    for (int y = 0; y < height; ++y) {
      // Build the string for a single row of voxels once
      std::string row_str;
      row_str.reserve(width * chars_per_voxel_x);

      for (int x = 0; x < width; ++x) {
        uint8_t brightness = ProjectZ(x, y);
        char ch = GetCharForBrightness(brightness);

        // Repeat character horizontally (n times)
        for (int n = 0; n < chars_per_voxel_x; ++n) {
          row_str += ch;
        }
      }

      // Print that horizontal row vertically (m times)
      for (int m = 0; m < chars_per_voxel_y; ++m) {
        frame_buffer += row_str;
        frame_buffer += "\n";
      }
    }

    std::cout << frame_buffer;
  }
};
