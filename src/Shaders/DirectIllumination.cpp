#include "Shaders/DirectIllumination.hpp"

#include "Light/Light.hpp"
#include "Math/Math.hpp"
#include "Math/Random.hpp"
#include "Primitive/BRDF.hpp"
#include "Primitive/Geometry/Mesh.hpp"
#include "Primitive/Material.hpp"
#include "Primitive/Primitive.hpp"
#include "Ray/Intersection.hpp"
#include "Ray/Ray.hpp"
#include "Scene/Scene.hpp"

#include <vector>

namespace VI {
namespace {

inline float Max3(const Vector &v) { return std::max(v.x, std::max(v.y, v.z)); }

float ComputeSpecularWeight(const Material &material) {
  const Vector f0 =
      glm::mix(Vector(0.04f), material.GetAlbedo(), material.GetMetallic());
  const float base_prob = Max3(f0);
  const float roughness_influence =
      glm::smoothstep(0.0f, 1.0f, material.GetRoughness() * 0.7f);
  return glm::mix(base_prob, base_prob * 0.5f, roughness_influence);
}

float ComputeDiffuseWeight(const Material &material) {
  return 1.0f - ComputeSpecularWeight(material);
}

struct AreaLightSample {
  Point Position;
  Vector Normal;
  float AreaPDF{0.f};
};

float ComputeTriangleArea(const Triangle &triangle) {
  const auto [v1, v2, v3] = triangle.GetVertices();
  return 0.5f * glm::length(glm::cross(v2 - v1, v3 - v1));
}

float ComputeMeshArea(const Mesh &mesh) {
  float total_area = 0.f;
  for (size_t i = 0; i < mesh.GetTriangleCount(); ++i) {
    total_area += ComputeTriangleArea(mesh.GetTriangle(i));
  }
  return total_area;
}

std::optional<AreaLightSample> SampleMeshAreaLight(const Mesh &mesh) {
  const float total_area = ComputeMeshArea(mesh);
  if (total_area <= EPSILON) {
    return std::nullopt;
  }

  const float target = Random::RandomFloat(0.f, 1.f) * total_area;
  float cumulative_area = 0.f;
  const Triangle *sampled_triangle = nullptr;

  for (size_t i = 0; i < mesh.GetTriangleCount(); ++i) {
    const Triangle &triangle = mesh.GetTriangle(i);
    cumulative_area += ComputeTriangleArea(triangle);
    if (target <= cumulative_area || i + 1 == mesh.GetTriangleCount()) {
      sampled_triangle = &triangle;
      break;
    }
  }

  if (sampled_triangle == nullptr) {
    return std::nullopt;
  }

  const auto [v1, v2, v3] = sampled_triangle->GetVertices();
  const float u1 = Random::RandomFloat(0.f, 1.f);
  const float u2 = Random::RandomFloat(0.f, 1.f);
  const float su1 = glm::sqrt(u1);
  const float b0 = 1.f - su1;
  const float b1 = su1 * (1.f - u2);
  const float b2 = su1 * u2;

  return AreaLightSample{
      .Position = b0 * v1 + b1 * v2 + b2 * v3,
      .Normal = glm::normalize(sampled_triangle->GetNormal()),
      .AreaPDF = 1.f / total_area,
  };
}

std::vector<const Light *> GetSupportedLights(const Scene &scene) {
  std::vector<const Light *> supported_lights{};
  supported_lights.reserve(scene.GetLights().size());

  for (const auto &light : scene.GetLights()) {
    const LightType light_type = light->GetType();
    if (light_type == LightType::Ambient || light_type == LightType::Point ||
        light_type == LightType::Area) {
      supported_lights.push_back(light.get());
    }
  }

  return supported_lights;
}

RGB EvaluateBSDF(const Vector &wo_local, const Vector &wi_local,
                 const Material &material) {
  const float specular_weight = ComputeSpecularWeight(material);
  const float diffuse_weight = ComputeDiffuseWeight(material);
  const LambertianBRDF lambertian{};
  const MicrofacetBRDF microfacet{};
  return diffuse_weight * lambertian.Evaluate(wo_local, wi_local, material) +
         specular_weight * microfacet.Evaluate(wo_local, wi_local, material);
}

} // namespace

std::optional<SelectedLight> SampleLightUniform(const Scene &scene) {
  const auto supported_lights = GetSupportedLights(scene);
  if (supported_lights.empty()) {
    return std::nullopt;
  }

  const int light_count = static_cast<int>(supported_lights.size());
  const int sampled_light_index =
      std::min(static_cast<int>(Random::RandomFloat(0.f, 1.f) * light_count),
               light_count - 1);

  return SelectedLight{
      .LightPtr = supported_lights[static_cast<size_t>(sampled_light_index)],
      .SelectionPDF = 1.f / static_cast<float>(light_count),
  };
}

RGB EstimateDirectIllumination(const Ray &ray, const Scene &scene,
                               const Intersection &intersection,
                               const Material &material,
                               const SelectedLight &selected_light) {
  if (selected_light.SelectionPDF <= 0.f ||
      selected_light.LightPtr == nullptr) {
    return RGB{0.f};
  }

  const Material &light_material =
      scene.GetMaterial(selected_light.LightPtr->GetMaterialIndex());
  const RGB light_radiance = light_material.GetRadiance();

  if (selected_light.LightPtr->GetType() == LightType::Ambient) {
    return (material.GetAlbedo() * light_radiance) /
           selected_light.SelectionPDF;
  }

  if (selected_light.LightPtr->GetType() == LightType::Point) {
    const auto *point_light =
        static_cast<const PointLight *>(selected_light.LightPtr);
    const Vector to_light = point_light->GetPosition() - intersection.Position;
    const float light_distance = glm::length(to_light);
    if (light_distance <= EPSILON) {
      return RGB{0.f};
    }

    const Vector wi_world = to_light / light_distance;
    const Ray shadow_ray =
        Ray::WithOffset(intersection.Position, wi_world, intersection.Normal);
    if (!scene.Visibility(shadow_ray, light_distance)) {
      return RGB{0.f};
    }

    const OrthonormalBasis basis{intersection.Normal};
    const Vector wo_world = -ray.Direction;
    const Vector wo_local = basis.WorldToLocal(wo_world);
    const Vector wi_local = basis.WorldToLocal(wi_world);

    if (wi_local.z <= 0.f || wo_local.z <= 0.f) {
      return RGB{0.f};
    }

    const RGB bsdf = EvaluateBSDF(wo_local, wi_local, material);

    return (bsdf * wi_local.z * light_radiance) / selected_light.SelectionPDF;
  }

  if (selected_light.LightPtr->GetType() == LightType::Area) {
    const auto *area_light =
        static_cast<const AreaLight *>(selected_light.LightPtr);
    const int object_index = area_light->GetObjectIndex();
    if (object_index < 0 ||
        static_cast<size_t>(object_index) >= scene.GetPrimitiveCount()) {
      return RGB{0.f};
    }

    const Primitive &light_primitive = scene.GetPrimitive(object_index);
    const auto *mesh = std::get_if<Mesh>(&light_primitive.Geometry);
    if (mesh == nullptr) {
      return RGB{0.f};
    }

    const auto area_sample = SampleMeshAreaLight(*mesh);
    if (!area_sample.has_value() || area_sample->AreaPDF <= 0.f) {
      return RGB{0.f};
    }

    const Vector to_light = area_sample->Position - intersection.Position;
    const float distance_squared = glm::dot(to_light, to_light);
    if (distance_squared <= EPSILON * EPSILON) {
      return RGB{0.f};
    }
    const float light_distance = glm::sqrt(distance_squared);
    const Vector wi_world = to_light / light_distance;
    const Vector shading_normal =
        FaceForward(intersection.Normal, -ray.Direction);

    const float cos_surface = glm::max(glm::dot(shading_normal, wi_world), 0.f);
    if (cos_surface <= 0.f) {
      return RGB{0.f};
    }

    const float cos_light =
        glm::max(glm::dot(area_sample->Normal, -wi_world), 0.f);
    if (cos_light <= 0.f) {
      return RGB{0.f};
    }

    const Ray shadow_ray =
        Ray::WithOffset(intersection.Position, wi_world, shading_normal);
    if (!scene.Visibility(shadow_ray, light_distance)) {
      return RGB{0.f};
    }

    const OrthonormalBasis basis{shading_normal};
    const Vector wo_world = -ray.Direction;
    const Vector wo_local = basis.WorldToLocal(wo_world);
    const Vector wi_local = basis.WorldToLocal(wi_world);
    if (wi_local.z <= 0.f || wo_local.z <= 0.f) {
      return RGB{0.f};
    }

    const RGB bsdf = EvaluateBSDF(wo_local, wi_local, material);
    const float geometry_term = (cos_surface * cos_light) / distance_squared;
    const float pdf = selected_light.SelectionPDF * area_sample->AreaPDF;
    if (pdf <= 0.f) {
      return RGB{0.f};
    }

    return (bsdf * light_radiance * geometry_term) / pdf;
  }

  return RGB{0.f};
}

RGB SampleDirectIlluminationUniform(const Ray &ray, const Scene &scene,
                                    const Intersection &intersection,
                                    const Material &material) {
  const std::optional<SelectedLight> selected_light = SampleLightUniform(scene);
  if (!selected_light.has_value()) {
    return RGB{0.f};
  }

  return EstimateDirectIllumination(ray, scene, intersection, material,
                                    *selected_light);
}

} // namespace VI
