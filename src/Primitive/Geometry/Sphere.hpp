#pragma once

#include "Math/Vector.hpp"
#include "Primitive/BoundingBox.hpp"
#include "Primitive/Geometry/Geometry.hpp"

namespace VI {
class Sphere final : public Geometry {
public:
  Sphere(Point center, float radius)
      : m_Center(center), m_Radius(radius),
        m_BoundingBox(center - radius, center + radius) {}

  bool Intersect(const Ray &r, Intersection &i) const override;

private:
  Point m_Center;
  float m_Radius;
  BoundingBox m_BoundingBox;
};
} // namespace VI
