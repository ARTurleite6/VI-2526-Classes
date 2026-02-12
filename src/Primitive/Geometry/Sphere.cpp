#include "Primitive/Geometry/Sphere.hpp"

#include <glm/geometric.hpp>

#include "Ray/Intersection.hpp"

namespace VI {

bool Sphere::Intersect(const Ray &ray,
                       Intersection &intersection [[maybe_unused]]) const {
  float tmin, tmax;
  if (!m_BoundingBox.Intersect(ray, tmin, tmax)) {
    return false;
  }

  return false;
}

} // namespace VI
