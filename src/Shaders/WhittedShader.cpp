#include "Shaders/WhittedShader.hpp"

#include "Camera/Camera.hpp"
#include "Math/Math.hpp"
#include "Math/Vector.hpp"
#include "Ray/Intersection.hpp"
#include "Ray/Ray.hpp"
#include "Scene/Scene.hpp"

namespace VI {

constexpr float MAX_DEPTH = 5;

RGB WhittedShader::Execute(int x, int y, const Scene &scene,
                           const Camera &camera) const {
  RGB color{0.0f};

  // Primary Ray
  Ray ray = camera.GenerateRay(x, y);

  for (int depth = 0; depth < MAX_DEPTH; depth++) {

    Intersection intersection{};
    if (!scene.Trace(ray, intersection)) {
      // color += m_BackgroundColor;
      break;
    }

    const Primitive &primitive = scene.GetPrimitive(intersection.ObjectIndex);
    const Material &material = scene.GetMaterial(primitive.MaterialIndex);

    for (const auto &light : scene.GetLights()) {
      const auto &light_material = scene.GetMaterial(light.MaterialIndex);

      if (light.Type == LightType::Ambient) {
        color += material.GetAlbedo() * light_material.GetRadiance();
      } else if (light.Type == LightType::Point) {
        Vector light_position = light.Position;
        Vector direction =
            glm::normalize(light_position - intersection.Position);
        float light_distance =
            glm::length(light_position - intersection.Position);

        if (float cos_light = glm::dot(direction, intersection.Normal);
            cos_light > 0) {
          Ray shadow_ray{intersection.Position + EPSILON * direction,
                         direction};

          Intersection shadow_intersection{};
          if (!scene.Trace(shadow_ray, shadow_intersection) ||
              shadow_intersection.Distance > light_distance - EPSILON) {
            color +=
                cos_light * material.GetAlbedo() * light_material.GetRadiance();
          }
        }
      }
    }

    if (material.GetRoughness() >= 1)
      break;

    // Reflection
    // Prepare reflected ray
    Vector reflected = glm::reflect(ray.Direction, intersection.Normal);
    Vector offset = EPSILON * intersection.Normal;
    if (glm::dot(reflected, intersection.Normal) < 0)
      offset *= -1.0f;
    ray.Origin = intersection.Position + offset;
    ray.Direction = reflected;
  }

  return color;
}

} // namespace VI
