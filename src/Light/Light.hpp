#pragma once

#include "Math/Vector.hpp"

namespace VI {
enum class LightType {
  Ambient,
  Point,
  Area,
};

struct Light {
  Point Position{};
  int MaterialIndex{-1};
  int ObjectIndex{-1};
  LightType Type;
};
} // namespace VI
