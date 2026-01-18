#include "Shaders/PathTracingShader.hpp"

#include "Camera/Camera.hpp"
#include "Math/Math.hpp"
#include "Math/RGB.hpp"
#include "Math/Random.hpp"
#include "Primitive/BRDF.hpp"
#include "Ray/Intersection.hpp"
#include "Scene/Scene.hpp"

namespace VI {
constexpr float MAX_DEPTH = 5;
constexpr int SAMPLES_PER_PIXEL = 32;

inline float max3(const Vector &v) { return std::max(v.x, std::max(v.y, v.z)); }

inline float GetSpecularProbability(const Material &material) {
  Vector F0 =
      glm::mix(Vector(0.04f), material.GetAlbedo(), material.GetMetallic());
  float baseProb = max3(F0);
  float roughnessInfluence =
      glm::smoothstep(0.0f, 1.0f, material.GetRoughness() * 0.7f);
  return glm::mix(baseProb, baseProb * 0.5f, roughnessInfluence);
}

RGB PathTracingShader::Execute(int x, int y, const Scene &scene,
                               const Camera &camera) const {

  RGB color{0.f};

  LambertianBRDF lambertian;
  MicrofacetBRDF microfacet;

  for (int spp = 0; spp < SAMPLES_PER_PIXEL; spp++) {
    RGB sample_color{0.f};
    RGB contribution{1.f};

    auto jitter = Random::RandomVec3(0, 1);
    auto ray = camera.GenerateRay(x, y, jitter);
    for (int depth = 0; depth < MAX_DEPTH; depth++) {
      Intersection intersection{};
      if (!scene.Trace(ray, intersection)) {
        sample_color += Vector{0.2, 0.2, 0.2} * contribution;
        break;
      }

      auto &object = scene.GetPrimitive(intersection.ObjectIndex);
      auto &material = scene.GetMaterial(object.MaterialIndex);

      sample_color += contribution * material.GetEmissionColor() *
                      material.GetEmissionPower();

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
        break;
      }

      Vector wi_world = basis.LocalToWorld(wi_local);
      contribution *= brdf * wi_local.z / pdf;

      ray =
          Ray::WithOffset(intersection.Position, wi_world, intersection.Normal);
    }

    color += sample_color;
  }

  return color / static_cast<float>(SAMPLES_PER_PIXEL);
}
} // namespace VI
