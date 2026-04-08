#include "Scene/Scene.hpp"

#include "Math/DiscreteDistribution.hpp"
#include "Math/Math.hpp"
#include "Primitive/Geometry/Geometry.hpp"
#include "Primitive/Geometry/Mesh.hpp"
#include "Ray/Intersection.hpp"
#include "Ray/Ray.hpp"

namespace VI {
namespace {

bool IsSupportedDirectLightType(const LightType light_type) {
  return light_type == LightType::Ambient || light_type == LightType::Point ||
         light_type == LightType::Area;
}

float ComputeLightWeight(const Scene &scene, const Light &light) {
  (void)scene;
  (void)light;
  // TODO(student): define the importance weight of each light using the known
  // source properties (e.g. area and emitted power/radiance).
  return 0.f;
}

LightSamplingDistribution BuildLightSamplingDistribution(const Scene &scene) {
  LightSamplingDistribution distribution{};
  std::vector<float> weights{};

  const auto &lights = scene.GetLights();
  distribution.LightIndices.reserve(lights.size());
  weights.reserve(lights.size());

  for (size_t light_index = 0; light_index < lights.size(); ++light_index) {
    const Light &light = *lights[light_index];
    if (!IsSupportedDirectLightType(light.GetType())) {
      continue;
    }

    distribution.LightIndices.push_back(static_cast<int>(light_index));
    weights.push_back(std::max(ComputeLightWeight(scene, light), 0.f));
  }

  distribution.PDF = NormalizeWeightsToPDF(weights);
  distribution.CDF = BuildCDF(distribution.PDF);
  return distribution;
}

} // namespace

void Scene::Build() {
  m_AccelerationStructure = GridAccelerationStructure::Create(*this);
  m_LightSamplingDistribution = BuildLightSamplingDistribution(*this);
}

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
