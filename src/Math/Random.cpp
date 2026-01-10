#include "Math/Random.hpp"

namespace VI {

Point Random::RandomInUnitDisk() {
  while (true) {
    Point p{s_UniformDist(s_Rng), s_UniformDist(s_Rng), 0.};

    if ((p.x * p.x + p.y * p.y) < 1.f) {
      return p;
    }
  }
}

std::mt19937 Random::s_Rng;
std::uniform_real_distribution<float> Random::s_UniformDist{-1., 1.};

} // namespace VI
