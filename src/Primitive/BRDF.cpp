#include "Primitive/BRDF.hpp"

#include <glm/glm.hpp>

#include "Math/RGB.hpp"
#include "Math/Random.hpp"
#include "Math/Vector.hpp"
#include "Primitive/Material.hpp"
#include "glm/ext/scalar_constants.hpp"

namespace VI {

constexpr float MIN_ROUGHNESS = 0.02f;

Vector LambertianBRDF::Sample(const Vector &wo_local [[maybe_unused]],
                              const Material &material [[maybe_unused]]) const {
  float u1 = Random::RandomFloat(0, 1);
  float u2 = Random::RandomFloat(0, 1);

  float r = glm::sqrt(u1);
  float theta = 2.0f * glm::pi<float>() * u2;

  float x = r * glm::cos(theta);
  float y = r * glm::sin(theta);
  float z = glm::sqrt(1.0f - u1);

  return Vector{x, y, z};
}

RGB LambertianBRDF::Evaluate(const Vector &wo_local [[maybe_unused]],
                             const Vector &wi_local [[maybe_unused]],
                             const Material &material) const {

  return material.GetAlbedo() / glm::pi<float>();
}

float LambertianBRDF::PDF(const Vector &wo_local [[maybe_unused]],
                          const Vector &wi_local,
                          const Material &material [[maybe_unused]]) const {
  if (wi_local.z <= 0.f) {
    return 0.f;
  }

  return wi_local.z / glm::pi<float>();
}

Vector MicrofacetBRDF::Sample(const Vector &wo_local,
                              const Material &material) const {
  if (wo_local.z <= 0.0f)
    return Vector{0.0f};

  float roughness = glm::max(material.GetRoughness(), MIN_ROUGHNESS);
  float a = roughness * roughness;

  Vector h = SampleGGX_VNDF(wo_local, a);
  Vector wi_local = glm::reflect(-wo_local, h);

  if (wi_local.z <= 0.0f)
    return Vector{0.0f};

  return wi_local;
}

RGB MicrofacetBRDF::Evaluate(const Vector &wo_local, const Vector &wi_local,
                             const Material &material) const {
  float NoV = wo_local.z;
  float NoL = wi_local.z;

  if (NoV <= 0.0f || NoL <= 0.0f)
    return RGB{0.0f};

  Vector h = glm::normalize(wo_local + wi_local);
  float NoH = glm::max(h.z, 0.0f);
  float VoH = glm::max(glm::dot(wo_local, h), 0.0f);

  float roughness = glm::max(material.GetRoughness(), MIN_ROUGHNESS);
  float a = roughness * roughness;

  float D = D_GGX(NoH, a);
  float G = G_Smith(NoV, NoL, a);

  RGB F0 = glm::mix(RGB{0.04f}, material.GetAlbedo(), material.GetMetallic());
  RGB F = Fresnel_Schlick(VoH, F0);

  return (D * G * F) / (4.0f * NoV * NoL);
}

float MicrofacetBRDF::PDF(const Vector &wo_local, const Vector &wi_local,
                          const Material &material) const {
  if (wo_local.z <= 0.0f || wi_local.z <= 0.0f)
    return 0.0f;

  Vector h = glm::normalize(wo_local + wi_local);

  float NoH = glm::max(h.z, 0.0f);
  float VoH = glm::max(glm::dot(wo_local, h), 1e-4f);
  float NoV = glm::max(wo_local.z, 1e-4f);

  float roughness = glm::max(material.GetRoughness(), MIN_ROUGHNESS);
  float a = roughness * roughness;

  float D = D_GGX(NoH, a);
  float G1 = G1_Smith(NoV, a);

  // VNDF PDF
  return (D * G1 * VoH) / NoV;
}

float MicrofacetBRDF::G1_Smith(float NoX, float a) const {
  float a2 = a * a;
  float denom = NoX + glm::sqrt(a2 + (1.0f - a2) * NoX * NoX);
  return (2.0f * NoX) / denom;
}

float MicrofacetBRDF::G_Smith(float NoV, float NoL, float a) const {
  return G1_Smith(NoV, a) * G1_Smith(NoL, a);
}

RGB MicrofacetBRDF::Fresnel_Schlick(float cosTheta, const RGB &F0) const {
  return F0 + (RGB{1.0f} - F0) * glm::pow(1.0f - cosTheta, 5.0f);
}

Vector MicrofacetBRDF::SampleGGX_VNDF(const Vector &wo, float a) const {
  // Stretch view
  Vector V = glm::normalize(Vector(a * wo.x, a * wo.y, wo.z));

  // Orthonormal basis
  float lensq = V.x * V.x + V.y * V.y;
  Vector T1 = lensq > 0.0f ? Vector(-V.y, V.x, 0.0f) / glm::sqrt(lensq)
                           : Vector(1.0f, 0.0f, 0.0f);
  Vector T2 = glm::cross(V, T1);

  float u1 = Random::RandomFloat(0.f, 1.f);
  float u2 = Random::RandomFloat(0.f, 1.f);

  float r = glm::sqrt(u1);
  float phi = 2.0f * glm::pi<float>() * u2;
  float t1 = r * glm::cos(phi);
  float t2 = r * glm::sin(phi);

  float s = 0.5f * (1.0f + V.z);
  t2 = (1.0f - s) * glm::sqrt(glm::max(0.0f, 1.0f - t1 * t1)) + s * t2;

  Vector H = t1 * T1 + t2 * T2 +
             glm::sqrt(glm::max(0.0f, 1.0f - t1 * t1 - t2 * t2)) * V;

  // Unstretch
  H = glm::normalize(Vector(a * H.x, a * H.y, glm::max(0.0f, H.z)));
  return H;
}

float MicrofacetBRDF::D_GGX(float NoH, float roughness) const {
  float a = glm::max(roughness, MIN_ROUGHNESS);
  float a2 = a * a;
  float nh = glm::clamp(NoH, 0.0f, 1.0f);
  float denom = nh * nh * (a2 - 1.0f) + 1.0f;
  return a2 / (glm::pi<float>() * denom * denom);
}

} // namespace VI
