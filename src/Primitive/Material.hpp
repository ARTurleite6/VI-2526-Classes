#pragma once

#include "Image/Image.hpp"
#include "Math/RGB.hpp"
#include "Math/Vector.hpp"

namespace VI {
class Material {
public:
  constexpr Material(std::string_view name, const RGB &albedo, float roughness,
                     float metallic, const RGB &emission_color = {},
                     float emission_power = 0.f,
                     std::optional<Image> albedo_image = std::nullopt)
      : m_Name{name}, m_Albedo{albedo}, m_EmissionColor{emission_color},
        m_Metallic{metallic}, m_Roughness{roughness},
        m_EmissionPower{emission_power},
        m_AlbedoTexture{std::move(albedo_image)} {}

  const std::string &GetName() const { return m_Name; }

  constexpr RGB GetAlbedo() const { return m_Albedo; }
  constexpr RGB GetAlbedo(int x, int y) const {
    if (m_AlbedoTexture.has_value()) {
      return m_AlbedoTexture->Get(x, y);
    }

    return m_Albedo;
  }

  constexpr RGB GetRadiance() const {
    return m_EmissionColor * m_EmissionPower;
  }

  constexpr float GetRoughness() const { return m_Roughness; }
  constexpr float GetMetallic() const { return m_Metallic; }

  constexpr RGB GetEmissionColor() const { return m_EmissionColor; }
  constexpr float GetEmissionPower() const { return m_EmissionPower; }
  constexpr Vector GetEmissionIntensity() const {
    return m_EmissionColor * m_EmissionPower;
  }

private:
  std::string m_Name;
  RGB m_Albedo, m_EmissionColor;
  float m_Metallic;
  float m_Roughness;
  float m_EmissionPower{0.f};

  std::optional<Image> m_AlbedoTexture{std::nullopt};
};
} // namespace VI
