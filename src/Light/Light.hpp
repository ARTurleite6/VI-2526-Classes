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

// class Light {
// public:
//   virtual ~Light() = default;
//   virtual LightType GetType() const = 0;
//   virtual RGB GetRadiance() const = 0;
// };
} // namespace VI
