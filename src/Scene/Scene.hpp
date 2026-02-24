#pragma once

#include "Light/Light.hpp"
#include "Primitive/AccelerationStructures/AccelerationStructure.hpp"
#include "Primitive/AccelerationStructures/GridAccelerationStructure.hpp"
#include "Primitive/Geometry/Geometry.hpp"
#include "Primitive/Material.hpp"
#include "Primitive/Primitive.hpp"

#include <memory>
#include <type_traits>

namespace VI {

struct Ray;
struct Intersection;

class Scene final {
public:
  void Build() {
    m_AccelerationStructure = GridAccelerationStructure::Create(*this);
  }

  bool Trace(const Ray &ray, Intersection &intersection) const;

  // Returns true if the path from ray origin to max_distance is unobstructed.
  bool Visibility(const Ray &ray, float max_distance) const;

  // TODO: check this to check if we should receive geometry as value, l-value
  // or r-value
  void AddPrimitive(Geometry primitive, int material_index) {
    assert(material_index >= 0 && material_index < m_Materials.size() &&
           "Material index out of range");

    m_Primitives.push_back(Primitive{.Geometry = std::move(primitive),
                                     .MaterialIndex = material_index});

    auto &material = m_Materials[material_index];
    if (material.GetEmissionPower() > 0.f) {
      m_Lights.emplace_back(std::make_unique<AreaLight>(
          material_index, static_cast<int>(m_Primitives.size())));
    }
  }

  inline int AddMaterial(MaterialDesc desc) {
    m_Materials.emplace_back(std::move(desc));
    return m_Materials.size() - 1;
  }

  inline void AddLight(std::unique_ptr<Light> light) {
    m_Lights.emplace_back(std::move(light));
  }

  inline const Primitive &GetPrimitive(int primitive_index) const {
    return m_Primitives[primitive_index];
  }

  inline const Material &GetMaterial(int material_index) const {
    return m_Materials[material_index];
  }

  inline const std::vector<std::unique_ptr<Light>> &GetLights() const {
    return m_Lights;
  }
  inline size_t GetPrimitiveCount() const { return m_Primitives.size(); }

  BoundingBox ComputeBoundingBox() const;

private:
  std::vector<Primitive> m_Primitives{};
  std::vector<Material> m_Materials{};
  std::vector<std::unique_ptr<Light>> m_Lights{};
  GridAccelerationStructure m_AccelerationStructure{};
};
} // namespace VI
