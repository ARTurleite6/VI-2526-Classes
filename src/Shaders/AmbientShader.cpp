#include "Shaders/AmbientShader.hpp"

#include "Camera/Camera.hpp"
#include "Math/RGB.hpp"
#include "Primitive/Primitive.hpp"
#include "Ray/Intersection.hpp"
#include "Scene/Scene.hpp"

namespace VI {
RGB AmbientShader::Execute(int x, int y, const Scene &scene,
                           const Camera &camera) const {
  Ray ray = camera.GenerateRay(x, y);

  Intersection intersection{};
  if (!scene.Trace(ray, intersection)) {
    return m_BackgroundColor;
  }

  const Primitive &primitive = scene.GetPrimitive(intersection.ObjectIndex);
  const Material &material = scene.GetMaterial(primitive.MaterialIndex);

  RGB color{0.f};

  for (const auto &light : scene.GetLights()) {
    const auto &light_material = scene.GetMaterial(light.MaterialIndex);

    if (light.Type == LightType::Ambient) {
      color += material.GetAlbedo() * light_material.GetRadiance();
    }
  }
  return color;
}

} // namespace VI
