#include "Scene/Scene.hpp"

namespace VI {

bool Scene::Trace(const Ray &ray, Intersection &intersection) const {
  for (const auto &primitive : m_Primitives) {
    // TODO: Implement ray tracing algorithm
    if (primitive.Geometry->Intersect(ray, intersection)) {
      return true;
    }
  }
  return false;
}

} // namespace VI
