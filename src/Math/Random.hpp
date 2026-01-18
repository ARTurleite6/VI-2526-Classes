#pragma once

#include "Math/Vector.hpp"

namespace VI {
class Random {
public:
  static float RandomFloat(float min = -1.f, float max = 1.f);
  static Vector RandomVec3(float min = -1.f, float max = 1.f);
  static Point RandomInUnitDisk();

private:
  static std::mt19937 s_Rng;
};
} // namespace VI
