#pragma once

#include "Math/Vector.hpp"
#include "Primitive/BoundingBox.hpp"
#include "Primitive/Geometry/Geometry.hpp"

namespace VI {
class Triangle final : public Geometry {
public:
  Triangle(const Point &v1, const Point &v2, const Point &v3,
           const Vector &normal, bool back_face_culling = false)
      : m_V1{v1}, m_V2{v2}, m_V3{v3}, m_Normal{normal},
        m_BackFaceCulling{back_face_culling} {
    m_BoundingBox = {.Min = v1, .Max = v1};
    m_BoundingBox.Update(v2);
    m_BoundingBox.Update(v3);
  }

  bool Intersect(const Ray &r, Intersection &i) const override;

  std::tuple<Point, Point, Point> GetVertices() const {
    return std::make_tuple(m_V1, m_V2, m_V3);
  }

private:
  Point m_V1, m_V2, m_V3;
  Vector m_Normal;
  BoundingBox m_BoundingBox;
  bool m_BackFaceCulling;
};
} // namespace VI
