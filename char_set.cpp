#include <cstdint>

#include "char_set.h"

char GetCharForBrightness(uint8_t brightness) {
  if (brightness == 0)
    return ' '; // Empty / completely transparent

  // Map the [1, 255] brightness value to an index in our inverted density
  // string
  int index = (brightness * (kDensity.length() - 1)) / 255;
  return kDensity[index];
}
