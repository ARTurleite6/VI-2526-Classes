#pragma once

#include "Math/Vector.hpp"

namespace VI {
constexpr float EPSILON = 1e-3f;

class OrthonormalBasis {
public:
  explicit OrthonormalBasis(const Vector &n);
  Vector WorldToLocal(const Vector &v) const;
  Vector LocalToWorld(const Vector &v) const;

private:
  Vector m_Tangent, m_Bitangent, m_Normal;
};

constexpr Vector OffsetPoint(const Point &point, const Vector &normal,
                             float epsilon = EPSILON) {
  return point + epsilon * normal;
}
} // namespace VI
