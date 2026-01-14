#pragma once

#include "Math/Vector.hpp"

namespace VI {
constexpr float EPSILON = 1e-3f;

constexpr Vector OffsetPoint(const Point &point, const Vector &normal,
                             float epsilon = EPSILON) {
  return point + epsilon * normal;
}
} // namespace VI
