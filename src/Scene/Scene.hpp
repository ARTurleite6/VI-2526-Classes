#pragma once

#include "Primitive/Geometry/Geometry.hpp"
#include "Primitive/Primitive.hpp"
#include "Ray/Intersection.hpp"
#include "Ray/Ray.hpp"

namespace VI {
class Scene {
public:
  bool Trace(const Ray &ray, Intersection &intersection) const;

  template <typename T>
    requires std::is_base_of_v<Geometry, T>
  void AddPrimitive(std::shared_ptr<T> primitive) {
    m_Primitives.emplace_back(std::move(primitive));
  }

private:
  std::vector<Primitive> m_Primitives{};
};
} // namespace VI
