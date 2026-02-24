#include "Shaders/WhittedShader.hpp"

#include "Math/Math.hpp"
#include "Math/RGB.hpp"
#include "Math/Vector.hpp"
#include "Ray/Intersection.hpp"
#include "Ray/Ray.hpp"
#include "Scene/Scene.hpp"

namespace VI {

constexpr float MAX_DEPTH = 5;

RGB WhittedShader::Execute(const Ray &ray, const Scene &scene) const {
  Intersection intersection{};
  if (scene.Trace(ray, intersection)) {
    return DoExecute(ray, scene, intersection);
  }

  return m_BackgroundColor;
}

RGB WhittedShader::DoExecute(const Ray &ray, const Scene &scene,
                             const Intersection &intersection,
                             int depth) const {
  if (depth > MAX_DEPTH) {
    return RGB{0.0f};
  }

  return DirectIllumination(ray, scene, intersection) +
         IndirectIllumination(ray, scene, intersection, depth + 1);
}

RGB WhittedShader::DirectIllumination(const Ray &ray, const Scene &scene,
                                      const Intersection &intersection) const {

  RGB color{0.0f};

  const Primitive &primitive = scene.GetPrimitive(intersection.ObjectIndex);
  const Material &material = scene.GetMaterial(primitive.MaterialIndex);
  Vector shading_normal = FaceForward(intersection.Normal, -ray.Direction);

  for (const auto &light : scene.GetLights()) {
    const auto &light_material = scene.GetMaterial(light->GetMaterialIndex());
    auto light_type = light->GetType();
    if (light_type == LightType::Ambient) {
      color += material.GetAlbedo() * light_material.GetRadiance();
    } else if (light_type == LightType::Point) {
      auto *point_light = static_cast<PointLight *>(light.get());
      const Vector light_position = point_light->GetPosition();
      const Vector direction =
          glm::normalize(light_position - intersection.Position);
      const float light_distance =
          glm::length(light_position - intersection.Position);

      if (const float cos_light = glm::dot(direction, shading_normal);
          cos_light > 0) {
        const auto shadow_ray = Ray::WithOffset(intersection.Position,
                                                direction, intersection.Normal);

        if (scene.Visibility(shadow_ray, light_distance)) {
          color +=
              cos_light * material.GetAlbedo() * light_material.GetRadiance();
        }
      }
    }
  }

  return color;
}

RGB WhittedShader::IndirectIllumination(const Ray &ray, const Scene &scene,
                                        const Intersection &intersection,
                                        int depth) const {
  return RGB{0.0f};
}
} // namespace VI
