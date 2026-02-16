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

  return DirectIllumination(scene, intersection) +
         IndirectIllumination(ray, scene, intersection, depth + 1);
}

RGB WhittedShader::DirectIllumination(const Scene &scene,
                                      const Intersection &intersection) const {

  RGB color{0.0f};

  const Primitive &primitive = scene.GetPrimitive(intersection.ObjectIndex);
  const Material &material = scene.GetMaterial(primitive.MaterialIndex);

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

      if (const float cos_light = glm::dot(direction, intersection.Normal);
          cos_light > 0) {
        const auto shadow_ray = Ray::WithOffset(intersection.Position,
                                                direction, intersection.Normal);

        Intersection shadow_intersection{};
        if (!scene.Trace(shadow_ray, shadow_intersection) ||
            shadow_intersection.Distance > light_distance - EPSILON) {
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
  const Primitive &primitive = scene.GetPrimitive(intersection.ObjectIndex);
  const Material &material = scene.GetMaterial(primitive.MaterialIndex);

  if (material.GetRoughness() == 0.0f) {
    return RGB{0.0f};
  }

  const Vector reflected = glm::reflect(ray.Direction, intersection.Normal);
  Ray scattered_ray =
      Ray::WithOffset(intersection.Position, reflected, intersection.Normal);

  Intersection scattered_intersection{};
  if (scene.Trace(scattered_ray, scattered_intersection)) {
    return RGB{0.0f};
  }

  return DoExecute(scattered_ray, scene, scattered_intersection, depth + 1);
}
} // namespace VI
