#include "Primitive/Geometry/Sphere.hpp"

#include <glm/geometric.hpp>

namespace VI {

bool Sphere::Intersect(const Ray &ray, Intersection &intersection) const {
  if (!m_BoundingBox.Intersect(ray)) {
    return false;
  }

  constexpr float EPSILON = std::numeric_limits<float>::epsilon();

  Vector oc = m_Center - ray.Origin;

  float h = glm::dot(ray.Direction, oc);
  float c = glm::dot(oc, oc) - (m_Radius * m_Radius);
  float discriminant = h * h - c;
  if (discriminant < EPSILON) {
    return false;
  }

  float t = h - std::sqrt(discriminant);

  if (t <= EPSILON) {
    return false;
  }

  Point hit_point = ray.Origin + t * ray.Direction;
  Vector normal = glm::normalize(hit_point - m_Center);

  intersection = {
      .Position = hit_point,
      .Normal = normal,
      .Distance = t,
  };

  return true;
}

} // namespace VI
