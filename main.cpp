/*
   This program displays a 3D voxel image using characters on a terminal and
   rotates it.

   Data structures:
   - Each frame is a set of dots in a 3-D grid.
   - Character sets are manually created into 4x2 (8-bit) pixel blocks.

   LwL
   2025-05-20 (Original)
   2026-07-05 (Updated with Gemini help)
   - Fixed a few bugs:
   - indices are not correctly set in for loop.
   - precison losing during computing.
   - Some cout stuff for better display performance
   - More character sets.
   - Image Loader
   */

#include <chrono>
#include <cmath>
#include <iostream>
#include <numbers>
#include <string>
#include <thread>

#include "image_loader.h"
#include "world.h"

int main(int argc, char *argv[]) {
  int frame_rate = 24;

  int width = 100;
  int height = 100;
  int depth = 100;

  // Set voxel display granularity (n, m)
  int chars_per_voxel_x = 2;
  int chars_per_voxel_y = 1;

  World original_world(width, height, depth, chars_per_voxel_x,
                       chars_per_voxel_y);
  World current_world(width, height, depth, chars_per_voxel_x,
                      chars_per_voxel_y);

  if (argc > 1) {
    std::string filepath = argv[1];
    ImageLoader loader;
    loader.LoadImageIntoWorld(filepath, original_world);
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  } else {
    for (int x = width / 3; x < (width / 3) * 2; ++x) {
      for (int y = height / 3; y < (height / 3) * 2; ++y) {
        original_world.at(x, y, depth / 2) = 200;
      }
    }
  }

  std::cout << "\x1b[2J";

  // Instead of angle, we track the desired width ratio [1.0 to -1.0]
  float width_ratio = 1.0f;
  float rotation_dir = -1.0f; // -1 shrinks the width, 1 grows the width
  float speed = 0.05f;        // How fast the width changes per frame

  // Show original image for 1 second
  current_world.Display();
  std::this_thread::sleep_for(frame_rate * std::chrono::milliseconds(1000 / frame_rate));

  while (true) {
    std::cout << "\x1b[H";

    // Calculate the exact angle needed to achieve the linear width
    float angle = 0.0f;
    if (rotation_dir < 0) {
      // 0 to Pi (Front face turning away)
      angle = std::acos(width_ratio);
    } else {
      // Pi to 2*Pi (Back face turning towards us)
      angle = 2.0f * std::numbers::pi_v<float> - std::acos(width_ratio);
    }

    current_world.RotateFrom(original_world, angle);
    current_world.Display();

    // Linearly adjust the width ratio and bounce at the extremes
    width_ratio += rotation_dir * speed;
    if (width_ratio <= -1.0f) {
      width_ratio = -1.0f;
      rotation_dir = 1.0f;
    } else if (width_ratio >= 1.0f) {
      width_ratio = 1.0f;
      rotation_dir = -1.0f;
    }

    if (std::abs(angle) <= 0.005) 
      std::this_thread::sleep_for(frame_rate * std::chrono::milliseconds(1000 / frame_rate));
    std::this_thread::sleep_for(std::chrono::milliseconds(1000 / frame_rate));
  }

  return 0;
}
