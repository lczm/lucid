#pragma once

#include <limits>

struct BoundingBox {
  float minX = std::numeric_limits<float>::max();
  float maxX = std::numeric_limits<float>::min();

  float minY = std::numeric_limits<float>::max();
  float maxY = std::numeric_limits<float>::min();

  float minZ = std::numeric_limits<float>::max();
  float maxZ = std::numeric_limits<float>::min();
};