#include "Math/Random.hpp"
#include <random>

namespace VI {

float Random::RandomFloat(float min, float max) {
  std::uniform_real_distribution<float> dist{min, max};
  return dist(s_Rng);
}

Vector Random::RandomVec3(float min, float max) {
  return {
      RandomFloat(min, max),
      RandomFloat(min, max),
      RandomFloat(min, max),
  };
}

Point Random::RandomInUnitDisk() {
  while (true) {
    Point p = RandomVec3();

    if ((p.x * p.x + p.y * p.y) < 1.f) {
      return p;
    }
  }
}

thread_local std::mt19937 Random::s_Rng;

} // namespace VI
