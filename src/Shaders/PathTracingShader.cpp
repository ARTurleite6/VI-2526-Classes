#include "Shaders/PathTracingShader.hpp"

#include "Math/Math.hpp"
#include "Math/RGB.hpp"
#include "Math/Vector.hpp"
#include "Primitive/BRDF.hpp"
#include "Primitive/Material.hpp"
#include "Primitive/Primitive.hpp"
#include "Ray/Intersection.hpp"
#include "Ray/Ray.hpp"
#include "Scene/Scene.hpp"
#include "Shaders/DirectIllumination.hpp"

#include <glm/common.hpp>
#include <glm/exponential.hpp>
#include <glm/geometric.hpp>
#include <algorithm>

namespace VI {
constexpr float MAX_DEPTH = 5;

inline float max3(const Vector &v) { return std::max(v.x, std::max(v.y, v.z)); }

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

  if (material.GetEmissionPower() > 0.0f) {
    return allow_emissive ? material.GetRadiance() : RGB{0.0f};
  }

  color +=
      IndirectIllumination(ray, scene, intersection, material, depth,
                           allow_emissive);

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

  if (material.GetMetallic() <= 0.f) {  // no specular, thus diffuse
    const LambertianBRDF lambertian{};
    const OrthonormalBasis basis{shading_normal};
    const Vector wo_local = basis.WorldToLocal(-ray.Direction);
    const Vector wi_local = lambertian.Sample(wo_local, material);
    const float pdf = lambertian.PDF(wo_local, wi_local, material);
    if (wo_local.z <= 0.f || wi_local.z <= 0.f || pdf <= 0.f) {
      return RGB{0.0f};
    }

    const RGB f = lambertian.Evaluate(wo_local, wi_local, material);
    const float cos_theta = wi_local.z;
    const Vector wi_world = glm::normalize(basis.LocalToWorld(wi_local));
    Ray scattered_ray =
        Ray::WithOffset(intersection.Position, wi_world, shading_normal);

    Intersection scattered_intersection{};
    RGB incoming_radiance = m_BackgroundColor;
    if (scene.Trace(scattered_ray, scattered_intersection)) {
      incoming_radiance = DoExecute(scattered_ray, scene, scattered_intersection,
                                    depth + 1, false);
    }

    return (f * incoming_radiance * cos_theta) / pdf;
  }
  else {    // Specular

      const Vector reflected = glm::reflect(ray.Direction, shading_normal);
      const float cos_theta =
      glm::max(0.0f, glm::dot(shading_normal, -ray.Direction));
      Ray scattered_ray =
      Ray::WithOffset(intersection.Position, reflected, shading_normal);

      const RGB r0 = material.GetAlbedo();
      const RGB fresnel = r0 + (RGB{1.f} - r0) * glm::pow(1.f - cos_theta, 5.f);

      Intersection scattered_intersection{};
      if (!scene.Trace(scattered_ray, scattered_intersection)) {
          return fresnel * m_BackgroundColor;
      }

      return fresnel * DoExecute(scattered_ray, scene, scattered_intersection,
                                 depth + 1, allow_emissive);
  }
}

} // namespace VI
