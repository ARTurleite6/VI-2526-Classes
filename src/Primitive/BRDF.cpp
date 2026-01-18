#include "Primitive/BRDF.hpp"

#include <glm/glm.hpp>

#include "Math/RGB.hpp"
#include "Math/Random.hpp"
#include "Math/Vector.hpp"
#include "Primitive/Material.hpp"
#include "glm/ext/scalar_constants.hpp"

namespace VI {

constexpr float MIN_ROUGHNESS = 0.02f;

Vector LambertianBRDF::Sample(const Vector &wo_local,
                              const Material &material) const {
  float u1 = Random::RandomFloat(0, 1);
  float u2 = Random::RandomFloat(0, 1);

  float r = glm::sqrt(u1);
  float theta = 2.0f * glm::pi<float>() * u2;

  float x = r * glm::cos(theta);
  float y = r * glm::sin(theta);
  float z = glm::sqrt(1.0f - u1);

  return Vector{x, y, z};
}

RGB LambertianBRDF::Evaluate(const Vector &wo_local, const Vector &wi_local,
                             const Material &material) const {

  float cosine = glm::max(0.f, wi_local.z);
  return material.GetAlbedo() / glm::pi<float>();
}

float LambertianBRDF::PDF(const Vector &wo_local, const Vector &wi_local,
                          const Material &material) const {
  if (wi_local.z <= 0.f) {
    return 0.f;
  }

  return wi_local.z / glm::pi<float>();
}

RGB MicrofacetBRDF::Sample(const Vector &wo_local,
                           const Material &material) const {
  Vector h = SampleGGX(material.GetRoughness());

  return glm::reflect(-wo_local, h);
}

RGB MicrofacetBRDF::Evaluate(const Vector &wo_local, const Vector &wi_local,
                             const Material &material) const {
  float NoL = wi_local.z;
  float NoV = wo_local.z;

  if (NoL <= 0.0f || NoV <= 0.0f) {
    return RGB{0.0f};
  }

  Vector h = glm::normalize(wo_local + wi_local);
  float NoH = h.z;
  float VoH = glm::dot(wo_local, h);

  float D = D_GGX(NoH, material.GetRoughness());

  float G = G_Smith(NoV, NoL, material.GetRoughness());
  Vector F0 =
      glm::mix(RGB{0.04f}, material.GetAlbedo(), material.GetMetallic());
  Vector F = Fresnel_Schlick(VoH, F0);

  return D * G * F / (4.0f * NoV * NoL);
}

float MicrofacetBRDF::PDF(const Vector &wo_local, const Vector &wi_local,
                          const Material &material) const {
  float roughness = glm::max(material.GetRoughness(), MIN_ROUGHNESS);
  Vector h = glm::normalize(wo_local + wi_local);
  float nh = glm::max(h.z, 1e-4f);
  float voh = glm::max(glm::dot(wo_local, h), 1e-4f);
  float D = D_GGX(nh, roughness);
  return glm::max(D * nh / (4.0f * voh), 1e-6f);
}

float MicrofacetBRDF::D_GGX(float NoH, float roughness) const {
  float a = glm::max(roughness, MIN_ROUGHNESS);
  float a2 = a * a;
  float nh = glm::clamp(NoH, 0.0f, 1.0f);
  float denom = nh * nh * (a2 - 1.0f) + 1.0f;
  return a2 / (glm::pi<float>() * denom * denom);
}

RGB MicrofacetBRDF::Fresnel_Schlick(float cos_theta, const RGB &F0) const {
  return F0 + (RGB{1.0f} - F0) * glm::pow(1.0f - cos_theta, 5.0f);
}

float MicrofacetBRDF::G_Smith(float NoV, float NoL, float roughness) const {
  float a = glm::max(roughness, MIN_ROUGHNESS);
  float k = a * 0.5f;
  float nv = glm::clamp(NoV, 1e-4f, 1.0f);
  float nl = glm::clamp(NoL, 1e-4f, 1.0f);
  float G1V = nv / (nv * (1.0f - k) + k);
  float G1L = nl / (nl * (1.0f - k) + k);
  return G1V * G1L;
}

Vector MicrofacetBRDF::SampleGGX(float roughness) const {
  float a = glm::max(roughness, MIN_ROUGHNESS);
  float u1 = Random::RandomFloat(0.f, 1.f);
  float u2 = Random::RandomFloat(0.f, 1.f);
  float phi = 2.0f * glm::pi<float>() * u1;
  float cos_theta = glm::sqrt((1.0f - u2) / (1.0f + (a * a - 1.0f) * u2));
  float sin_theta = glm::sqrt(1.0f - cos_theta * cos_theta);
  return Vector(sin_theta * glm::cos(phi), sin_theta * glm::sin(phi),
                cos_theta);
}
} // namespace VI
