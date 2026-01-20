#include "Primitive/Geometry/Triangle.hpp"

#include "Math/Math.hpp"
#include "Ray/Intersection.hpp"
#include "Ray/Ray.hpp"

namespace VI {

bool Triangle::Intersect(const Ray &r, Intersection &intersection) const {
  float tmin, tmax;
  if (!m_BoundingBox.Intersect(r, tmin, tmax)) {
    return false;
  }
  const float par = glm::dot(m_Normal, r.Direction);
  if ((m_BackFaceCulling && par > -EPSILON) ||
      (!m_BackFaceCulling && glm::abs(par) < EPSILON)) {
    return false;
  }

  Vector edge1 = m_V2 - m_V1;
  Vector edge2 = m_V3 - m_V1;

  Vector h, s, q;
  float a, ff, u, v;

  h = glm::cross(r.Direction, edge2);
  a = glm::dot(edge1, h);
  ff = 1.0 / a;
  s = r.Origin - m_V1;
  u = ff * glm::dot(s, h);

  if (u < 0.0 || u > 1.0) {
    return false;
  }

  q = glm::cross(s, edge1);
  v = ff * glm::dot(r.Direction, q);

  if (v < 0.0 || u + v > 1.0) {
    return false;
  }

  float t = ff * glm::dot(edge2, q);
  if (t <= EPSILON)
    return false;

  Point p_hit = r.Origin + t * r.Direction;

  intersection = {
      .Position = p_hit,
      .Normal = m_Normal,
      .Distance = t,
  };

  return true;
}
} // namespace VI
