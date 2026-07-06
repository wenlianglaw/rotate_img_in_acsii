#pragma once
#include <cstdint>
#include <string>

// ---------------------------------------------------------
// Character Set & Mapping (Grayscale Density)
// ---------------------------------------------------------
// Reversed for white backgrounds:
// Starts with dense characters (darkest) and ends with sparse characters
// (brightest).
const std::string kDensity =
    "$@B%8&WM#*oahkbdpqwmZO0QLCJUYXzcvunxrjft/\\|()1{}[]?-_+~<>i!lI;:,\"`'.";

char GetCharForBrightness(uint8_t brightness);
