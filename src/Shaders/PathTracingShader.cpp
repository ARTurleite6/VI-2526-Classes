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
                                 const Intersection &intersection,
                                 int depth, bool allow_emissive) const {

  if (depth > MAX_DEPTH)
    return RGB{0.f};

  return DirectIllumination(ray, scene, intersection) +
         IndirectIllumination(ray, scene, intersection, depth, allow_emissive);
}

RGB PathTracingShader::DirectIllumination(const Ray &ray, const Scene &scene,
                                          const Intersection &intersection) const {
  const auto &object = scene.GetPrimitive(intersection.ObjectIndex);
  const auto &material = scene.GetMaterial(object.MaterialIndex);

  return SampleDirectIlluminationUniform(ray, scene, intersection, material);
}

RGB PathTracingShader::IndirectIllumination(const Ray &ray, const Scene &scene,
                                            const Intersection &intersection,
                                            int depth,
                                            bool allow_emissive) const {
  RGB color{0.0f};
  LambertianBRDF lambertian;
  MicrofacetBRDF microfacet;

  auto &object = scene.GetPrimitive(intersection.ObjectIndex);
  auto &material = scene.GetMaterial(object.MaterialIndex);

  if (allow_emissive) {
    color += material.GetEmissionColor() * material.GetRadiance();
  }

  float specular_weight = GetSpecularProbability(material);
  float diffuse_weight = 1.0f - specular_weight;

  OrthonormalBasis basis{intersection.Normal};
  Vector wo_world = -ray.Direction;
  Vector wo_local = basis.WorldToLocal(wo_world);

  Vector wi_local;
  float pdf = 0.0f;
  bool next_allow_emissive = false;

  if (Random::RandomFloat(0, 1) < diffuse_weight) {
    wi_local = lambertian.Sample(wo_local, material);
    float pdf_l = lambertian.PDF(wo_local, wi_local, material);
    float pdf_s = microfacet.PDF(wo_local, wi_local, material);
    pdf = diffuse_weight * pdf_l + specular_weight * pdf_s;
    next_allow_emissive = false;
  } else {
    wi_local = microfacet.Sample(wo_local, material);
    float pdf_l = lambertian.PDF(wo_local, wi_local, material);
    float pdf_s = microfacet.PDF(wo_local, wi_local, material);
    pdf = diffuse_weight * pdf_l + specular_weight * pdf_s;
    next_allow_emissive = true;
  }

  if (pdf <= 0.0f || glm::isnan(pdf) || wi_local.z <= 0) {
    return RGB{0.f};
  }

  RGB brdf = diffuse_weight * lambertian.Evaluate(wo_local, wi_local, material) +
             specular_weight * microfacet.Evaluate(wo_local, wi_local, material);

  Vector wi_world = basis.LocalToWorld(wi_local);

  Ray scattered_ray =
      Ray::WithOffset(intersection.Position, wi_world, intersection.Normal);

  Intersection scatter_intersection{};
  if (scene.Trace(scattered_ray, scatter_intersection)) {
    RGB rcolor =
        DoExecute(scattered_ray, scene, scatter_intersection, depth + 1,
                  next_allow_emissive);

    color += (brdf * wi_local.z * rcolor) / pdf;
  } else {
    color += (brdf * wi_local.z * m_BackgroundColor) / pdf;
  }

  return color;
}

} // namespace VI
