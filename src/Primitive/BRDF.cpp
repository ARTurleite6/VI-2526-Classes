#include "Primitive/BRDF.hpp"

#include <glm/glm.hpp>

#include "Math/RGB.hpp"
#include "Math/Random.hpp"
#include "Math/Vector.hpp"
#include "Primitive/Material.hpp"
#include "glm/ext/scalar_constants.hpp"

namespace VI {

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
  return material.GetAlbedo() * cosine / glm::pi<float>();
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
  if (wo_local.z <= 0.f || wi_local.z <= 0.f) {
    return RGB{0.f, 0.f, 0.f};
  }

  RGB F0;
  {
    const RGB dielectric{0.04f};
    float metallic = material.GetMetallic();
    F0 = dielectric * (1.f - metallic) + material.GetAlbedo() * metallic;
  }

  float roughness = material.GetRoughness();

  Vector h = wo_local + wi_local;
  float h_len = glm::length(h);
  if (h_len < 1e-6f)
    return RGB{0.f, 0.f, 0.f};
  h /= h_len;

  float D = D_GGX(h, roughness);
  RGB F = Fresnel_Schlick(glm::dot(wi_local, h), F0);
  float G = G_Smith(wo_local, wi_local, roughness);

  float denom = 4.f * wo_local.z * wi_local.z;
  if (denom < 1e-6f)
    return RGB{0.f, 0.f, 0.f};

  return F * D * G / denom;
}

float MicrofacetBRDF::PDF(const Vector &wo_local, const Vector &wi_local,
                          const Material &material) const {
  if (wo_local.z <= 0.0f || wi_local.z <= 0.0f)
    return 0.0f;

  Vector h = glm::normalize(wo_local + wi_local);
  float wo_dot_h = glm::dot(wo_local, h);

  if (wo_dot_h <= 1e-6f)
    return 0.0f;

  float D = D_GGX(h, material.GetRoughness());
  float pdf_h = D * h.z / (4.0f * wo_dot_h);

  if (glm::isnan(pdf_h) || pdf_h < 0.0f)
    return 0.0f;

  return pdf_h;
}

float MicrofacetBRDF::D_GGX(const Vector &h, float alpha) const {
  float a2 = alpha * alpha;
  float cos2 = h.z * h.z;
  float denom = cos2 * (a2 - 1.0f) + 1.0f;
  return a2 / (glm::pi<float>() * denom * denom);
}

RGB MicrofacetBRDF::Fresnel_Schlick(float cosTheta, const RGB &F0) const {
  return F0 + (RGB(1.0f) - F0) * powf(1.0f - cosTheta, 5.0f);
}

float MicrofacetBRDF::G_Smith(const Vector &wo, const Vector &wi,
                              float alpha) const {
  return G_SchlickGGX(wo.z, alpha) * G_SchlickGGX(wi.z, alpha);
}

float MicrofacetBRDF::G_SchlickGGX(float cosTheta, float alpha) const {
  float k = (alpha + 1.0f) * (alpha + 1.0f) / 8.0f;
  return cosTheta / (cosTheta * (1.0f - k) + k);
}

Vector MicrofacetBRDF::SampleGGX(float alpha) const {
  float u1 = Random::RandomFloat(0.f, 1.f);
  float u2 = Random::RandomFloat(0.f, 1.f);
  float phi = 2.0f * glm::pi<float>() * u1;
  float cosTheta = sqrtf((1.0f - u2) / (1.0f + (alpha * alpha - 1.0f) * u2));
  float sinTheta = sqrtf(1.0f - cosTheta * cosTheta);
  return Vector(sinTheta * cos(phi), sinTheta * sin(phi), cosTheta);
}
} // namespace VI
