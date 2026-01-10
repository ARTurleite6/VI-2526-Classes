#pragma once

#include "Math/Vector.hpp"

namespace VI {
class Random {
public:
  static Point RandomInUnitDisk();

private:
  static std::mt19937 s_Rng;
  static std::uniform_real_distribution<float> s_UniformDist;
};
} // namespace VI
