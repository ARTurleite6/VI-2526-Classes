#pragma once

#include "Light/Light.hpp"
#include "Primitive/Geometry/Geometry.hpp"
#include "Primitive/Material.hpp"
#include "Primitive/Primitive.hpp"

namespace VI {

class Ray;
class Intersection;

class Scene final {
public:
  bool Trace(const Ray &ray, Intersection &intersection) const;

  // TODO: check this to check if we should receive geometry as value, l-value
  // or r-value
  void AddPrimitive(Geometry primitive, int material_index) {
    assert(material_index >= 0 && material_index < m_Materials.size() &&
           "Material index out of range");

    m_Primitives.push_back(Primitive{.Geometry = std::move(primitive),
                                     .MaterialIndex = material_index});
  }

  int AddMaterial(const Material &material) {
    m_Materials.push_back(material);
    return m_Materials.size() - 1;
  }

  void AddLight(const Light &light) { m_Lights.emplace_back(light); }

  const Primitive &GetPrimitive(int primitive_index) const {
    return m_Primitives[primitive_index];
  }

  const Material &GetMaterial(int material_index) const {
    return m_Materials[material_index];
  }

  const std::vector<Light> &GetLights() const { return m_Lights; }

private:
  std::vector<Primitive> m_Primitives{};
  std::vector<Material> m_Materials{};
  std::vector<Light> m_Lights{};
};
} // namespace VI
