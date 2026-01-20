#pragma once

#include "Primitive/AccelerationStructures/AccelerationStructure.hpp"
#include "Primitive/BoundingBox.hpp"

namespace VI {
struct GridCell {
  std::vector<int> ObjectIndices;
  BoundingBox BoundingBox;
};

class GridAccelerationStructure final : public AccelerationStructure {
public:
  static GridAccelerationStructure Create(const Scene &scene);
  bool Trace(const Ray &ray, const Scene &scene,
             Intersection &intersection) const override;
  void Build(const Scene &scene) override;

private:
  std::vector<GridCell> m_Cells{};
  int m_Nx{0}, m_Ny{0}, m_Nz{0};
  BoundingBox m_BoundingBox{};
  float m_CellSizeX{0.0f}, m_CellSizeY{0.0f}, m_CellSizeZ{0.0f};
};

} // namespace VI
