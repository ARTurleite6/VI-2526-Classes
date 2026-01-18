#include "Scene/Scene.hpp"

#include "Ray/Intersection.hpp"
#include "Ray/Ray.hpp"

namespace VI {

bool Scene::Trace(const Ray &ray, Intersection &intersection) const {
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

} // namespace VI
