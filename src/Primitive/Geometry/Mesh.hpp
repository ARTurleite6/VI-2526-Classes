#pragma once

#include "Primitive/BoundingBox.hpp"
#include "Primitive/Geometry/Triangle.hpp"

namespace VI {

struct Ray;
struct Intersection;

class Mesh final {
public:
  Mesh(std::string_view name, const std::vector<Triangle> triangles)
      : m_Name{name}, m_Triangles{triangles} {
    // Compute bounding box from triangles
    if (!m_Triangles.empty()) {
      m_BoundingBox = BoundingBox{};
      for (const auto &tri : m_Triangles) {
        const auto &[v1, v2, v3] = tri.GetVertices();
        m_BoundingBox.Update(v1);
        m_BoundingBox.Update(v2);
        m_BoundingBox.Update(v3);
      }
    }
  }

  bool Intersect(const Ray &r, Intersection &i) const;
  inline const BoundingBox &GetBoundingBox() const { return m_BoundingBox; }

  void AddTriangle(const Triangle &triangle) {
    m_Triangles.push_back(triangle);
  }

private:
  std::string m_Name;
  std::vector<Triangle> m_Triangles{};
  BoundingBox m_BoundingBox{};
};
} // namespace VI
