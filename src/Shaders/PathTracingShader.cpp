#include "Shaders/PathTracingShader.hpp"

#include "Math/Math.hpp"
#include "Math/RGB.hpp"
#include "Math/Random.hpp"
#include "Primitive/BRDF.hpp"
#include "Ray/Intersection.hpp"
#include "Scene/Scene.hpp"
#include "Shaders/DirectIllumination.hpp"

namespace VI {
constexpr float MAX_DEPTH = 10;

inline float max3(const Vector &v) { return std::max(v.x, std::max(v.y, v.z)); }

inline float GetSpecularProbability(const Material &material) {
  Vector F0 =
      glm::mix(Vector(0.04f), material.GetAlbedo(), material.GetMetallic());
  float baseProb = max3(F0);
  float roughnessInfluence =
      glm::smoothstep(0.0f, 1.0f, material.GetRoughness() * 0.7f);
  return glm::mix(baseProb, baseProb * 0.5f, roughnessInfluence);
}

RGB PathTracingShader::Execute(const Ray &ray, const Scene &scene) const {
  Intersection intersection{};
  if (!scene.Trace(ray, intersection)) {
    return m_BackgroundColor;
  }

  return DoExecute(ray, scene, intersection);
}

RGB PathTracingShader::DoExecute(const Ray &ray, const Scene &scene,
                                 const Intersection &intersection, int depth,
                                 bool allow_emissive) const {
  RGB color{0.0f};
  if (depth > MAX_DEPTH) {
    return color;
  }

  const Primitive &primitive = scene.GetPrimitive(intersection.ObjectIndex);
  const Material &material = scene.GetMaterial(primitive.MaterialIndex);

  if (allow_emissive && material.GetEmissionPower() > 0.0f) {
      return material.GetRadiance();
  }

  if (material.GetMetallic() > 0.f) {
    color += IndirectIllumination(ray, scene, intersection, material, depth,
                                  allow_emissive);
  }

  return color + DirectIllumination(ray, scene, intersection);
}

RGB PathTracingShader::DirectIllumination(
    const Ray &ray, const Scene &scene,
    const Intersection &intersection) const {
  const auto &object = scene.GetPrimitive(intersection.ObjectIndex);
  const auto &material = scene.GetMaterial(object.MaterialIndex);

  return SampleDirectIllumination(ray, scene, intersection, material,
                                  m_DirectIlluminationMode);
}

RGB PathTracingShader::IndirectIllumination(const Ray &ray, const Scene &scene,
                                            const Intersection &intersection,
                                            const Material &material, int depth,
                                            bool allow_emissive) const {
  const Vector shading_normal =
      FaceForward(intersection.Normal, -ray.Direction);
  const Vector reflected = glm::reflect(ray.Direction, shading_normal);
  const float cos_theta =
      glm::max(0.0f, glm::dot(shading_normal, -ray.Direction));
  Ray scattered_ray =
      Ray::WithOffset(intersection.Position, reflected, intersection.Normal);

  const RGB r0 = material.GetAlbedo();
  const RGB fresnel = r0 + (RGB{1.f} - r0) * glm::pow(1.f - cos_theta, 5.f);

  Intersection scattered_intersection{};
  if (!scene.Trace(scattered_ray, scattered_intersection)) {
    return fresnel * m_BackgroundColor;
  }

  return fresnel * DoExecute(scattered_ray, scene, scattered_intersection,
                             depth + 1, allow_emissive);
}

} // namespace VI
