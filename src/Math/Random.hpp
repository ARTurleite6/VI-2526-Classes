#pragma once

#include "Math/Vector.hpp"

namespace VI {
class Random {
public:
  static float RandomFloat(int min = -1.f, int max = 1.f);
  static Vector RandomVec3(int min = -1.f, int max = 1.f);
  static Point RandomInUnitDisk();

private:
  static std::mt19937 s_Rng;
  static std::uniform_real_distribution<float> s_UniformDist;
};
} // namespace VI
