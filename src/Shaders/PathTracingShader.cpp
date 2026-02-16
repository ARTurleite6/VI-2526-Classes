#include "Shaders/PathTracingShader.hpp"

#include "Math/Math.hpp"
#include "Math/RGB.hpp"
#include "Math/Random.hpp"
#include "Primitive/BRDF.hpp"
#include "Ray/Intersection.hpp"
#include "Scene/Scene.hpp"

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
  if (scene.Trace(ray, intersection)) {
    return DoExecute(ray, scene, intersection);
  }

  return RGB{0.4f};
}

RGB PathTracingShader::DoExecute(const Ray &ray, const Scene &scene,
                                 const Intersection &intersection,
                                 int depth) const {

  if (depth > MAX_DEPTH)
    return RGB{0.f};

  return IndirectIllumination(ray, scene, intersection, depth);
}

RGB PathTracingShader::IndirectIllumination(const Ray &ray, const Scene &scene,
                                            const Intersection &intersection,
                                            int depth) const {
  RGB color{0.0f};
  LambertianBRDF lambertian;
  MicrofacetBRDF microfacet;

  auto &object = scene.GetPrimitive(intersection.ObjectIndex);
  auto &material = scene.GetMaterial(object.MaterialIndex);

  color += material.GetEmissionColor() * material.GetRadiance();

  OrthonormalBasis basis{intersection.Normal};
  Vector wo_world = -ray.Direction;
  Vector wo_local = basis.WorldToLocal(wo_world);

  float specular_weight = GetSpecularProbability(material);
  float diffuse_weight = 1.0f - specular_weight;

  Vector wi_local;
  float pdf = 0.0f;
  RGB brdf;

  if (Random::RandomFloat(0, 1) < diffuse_weight) {
    wi_local = lambertian.Sample(wo_local, material);
    float pdf_l = lambertian.PDF(wo_local, wi_local, material);
    float pdf_s = microfacet.PDF(wo_local, wi_local, material);
    pdf = diffuse_weight * pdf_l + specular_weight * pdf_s;
    brdf = lambertian.Evaluate(wo_local, wi_local, material);
  } else {
    wi_local = microfacet.Sample(wo_local, material);
    float pdf_l = lambertian.PDF(wo_local, wi_local, material);
    float pdf_s = microfacet.PDF(wo_local, wi_local, material);
    pdf = diffuse_weight * pdf_l + specular_weight * pdf_s;
    brdf = microfacet.Evaluate(wo_local, wi_local, material);
  }

  if (pdf <= 0.0f || glm::isnan(pdf) || wi_local.z <= 0) {
    return RGB{0.f};
  }

  Vector wi_world = basis.LocalToWorld(wi_local);

  Ray scattered_ray =
      Ray::WithOffset(intersection.Position, wi_world, intersection.Normal);

  Intersection scatter_intersection{};
  if (scene.Trace(scattered_ray, scatter_intersection)) {
    RGB rcolor =
        DoExecute(scattered_ray, scene, scatter_intersection, depth + 1);

    color += (brdf * wi_local.z * rcolor) / pdf;
  }

  return color;
}

} // namespace VI
