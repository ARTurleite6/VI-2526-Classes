#include "Scene/Scene.hpp"

#include "Math/Math.hpp"
#include "Primitive/Geometry/Geometry.hpp"
#include "Ray/Intersection.hpp"
#include "Ray/Ray.hpp"

namespace VI {

bool Scene::Trace(const Ray &ray, Intersection &intersection) const {
  // return m_AccelerationStructure.Trace(ray, *this, intersection);
  intersection.Distance = -1;

  for (size_t i = 0; i < m_Primitives.size(); i++) {
    const auto &primitive = m_Primitives[i];

    Intersection temp_intersection{};

    if (Intersect(primitive.Geometry, ray, temp_intersection)) {
      if (intersection.Distance == -1 ||
          intersection.Distance > temp_intersection.Distance) {
        intersection = temp_intersection;
        intersection.ObjectIndex = i;
      }
    }
  }

  return intersection.Distance != -1;
}

bool Scene::Visibility(const Ray &ray, float max_distance) const {
  for (const auto &primitive : m_Primitives) {
    Intersection intersection{};
    if (Intersect(primitive.Geometry, ray, intersection) &&
        intersection.Distance < max_distance - EPSILON) {
      return false;
    }
  }
  return true;
}

BoundingBox Scene::ComputeBoundingBox() const {
  if (m_Primitives.empty()) {
    return BoundingBox{};
  }

  BoundingBox bounding_box = GetBoundingBox(m_Primitives[0].Geometry);
  for (size_t i = 1; i < m_Primitives.size(); ++i) {
    bounding_box.Update(GetBoundingBox(m_Primitives[i].Geometry));
  }

  return bounding_box;
}

} // namespace VI
