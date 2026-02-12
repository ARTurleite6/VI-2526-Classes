#pragma once

#include "Math/Vector.hpp"
#include "Primitive/BoundingBox.hpp"

namespace VI {
struct Ray;
struct Intersection;

class Sphere final {
public:
  Sphere(Point center, float radius)
      : m_Center(center), m_Radius(radius),
        m_BoundingBox(center - radius, center + radius) {}

  bool Intersect(const Ray &r, Intersection &i) const;
  inline const BoundingBox &GetBoundingBox() const { return m_BoundingBox; }

private:
  Point m_Center [[maybe_unused]];
  float m_Radius [[maybe_unused]];
  BoundingBox m_BoundingBox [[maybe_unused]];
};
} // namespace VI
