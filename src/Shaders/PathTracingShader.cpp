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
constexpr int SAMPLES_PER_PIXEL = 10;

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

      if (material.GetEmissionPower() > 0.0f) {
        sample_color += contribution * material.GetRadiance();
        break;
      }

      OrthonormalBasis basis{intersection.Normal};
      Vector wo_world = -ray.Direction;
      Vector wo_local = basis.WorldToLocal(wo_world);

      Vector wi_local = lambertian.Sample(wo_local, material);
      float pdf = lambertian.PDF(wo_local, wi_local, material);
      if (pdf < 1e-6f)
        break;

      Vector wi_world = basis.LocalToWorld(wi_local);

      RGB brdf_lambertian = lambertian.Evaluate(wo_local, wi_local, material);

      contribution *= brdf_lambertian / pdf;
      ray =
          Ray::WithOffset(intersection.Position, wi_world, intersection.Normal);
    }

    color += sample_color;
  }

  return color / static_cast<float>(SAMPLES_PER_PIXEL);
}
} // namespace VI
