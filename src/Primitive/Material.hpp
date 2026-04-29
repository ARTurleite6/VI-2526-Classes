#pragma once

#include "Image/Image.hpp"
#include "Math/RGB.hpp"
#include "Math/Vector.hpp"

#include <algorithm>
#include <cmath>
#include <optional>
#include <string>
#include <string_view>
#include <utility>
#include <glm/common.hpp>

namespace VI
{

enum class TextureWrapMode
{
  Repeat,
  ClampToEdge,
  MirroredRepeat,
};

enum class TextureFilterMode
{
  Nearest,
  Linear,
};

struct TextureSampler
{
  Image Data;
  TextureWrapMode WrapS{TextureWrapMode::Repeat};
  TextureWrapMode WrapT{TextureWrapMode::Repeat};
  TextureFilterMode Filter{TextureFilterMode::Linear};

  RGB Sample(const Vec2& uv) const
  {
    // TODO(texture-mapping exercise):
    // 1. Wrap uv.x with WrapS and uv.y with WrapT.
    // 2. If Filter is Nearest, sample the closest texel.
    // 3. Otherwise, linearly blend the four neighboring texels.
    //
    // Until this is implemented, textures have no visible effect because
    // multiplying a material color by white leaves it unchanged.
    (void)uv;
    return RGB{1.0f};
  }

private:
  static float ApplyWrap(float value, TextureWrapMode mode)
  {
    // UVs are normalized texture coordinates, but they can be outside the
    // [0, 1] range. The wrap mode defines how those out-of-range values map
    // back onto the finite texture image.
    switch (mode)
    {
      case TextureWrapMode::ClampToEdge:
        // Clamp keeps sampling at the nearest texture edge.
        return glm::clamp(value, 0.0f, 1.0f);
      case TextureWrapMode::MirroredRepeat:
      {
        // Mirrored repeat behaves like repeat, but flips direction every
        // integer interval: 0..1 goes forward, 1..2 goes backward, etc.
        const float wrapped = value - std::floor(value);
        const auto period = static_cast<int>(std::floor(value));
        return period % 2 == 0 ? wrapped : 1.0f - wrapped;
      }
      case TextureWrapMode::Repeat:
        // Repeat keeps only the fractional part, so 1.25 samples like 0.25.
        return value - std::floor(value);
    }

    return value;
  }

  RGB SampleNearest(float u, float v) const
  {
    // Nearest filtering maps the normalized UV coordinate directly to the
    // closest discrete texel. The min guards handle u/v == 1, which would
    // otherwise produce an index one past the last valid pixel.
    const int x = std::min(static_cast<int>(u * Data.GetWidth()), Data.GetWidth() - 1);
    const int y = std::min(static_cast<int>(v * Data.GetHeight()), Data.GetHeight() - 1);
    return Data.Get(x, y);
  }

  RGB SampleLinear(float u, float v) const
  {
    // Linear filtering treats the texture as a continuous surface. First map
    // the normalized UV into image coordinates where integer positions are
    // texel centers.
    const float x = u * static_cast<float>(Data.GetWidth() - 1);
    const float y = v * static_cast<float>(Data.GetHeight() - 1);

    // Select the four texels around the sample position.
    const int x0 = static_cast<int>(std::floor(x));
    const int y0 = static_cast<int>(std::floor(y));
    const int x1 = std::min(x0 + 1, Data.GetWidth() - 1);
    const int y1 = std::min(y0 + 1, Data.GetHeight() - 1);

    // tx and ty are the fractional distances between the left/right and
    // bottom/top texels. They become the interpolation weights.
    const float tx = x - static_cast<float>(x0);
    const float ty = y - static_cast<float>(y0);

    const RGB c00 = Data.Get(x0, y0);
    const RGB c10 = Data.Get(x1, y0);
    const RGB c01 = Data.Get(x0, y1);
    const RGB c11 = Data.Get(x1, y1);

    // Bilinear interpolation: blend horizontally on both rows, then blend
    // between those two row colors vertically.
    return glm::mix(glm::mix(c00, c10, tx), glm::mix(c01, c11, tx), ty);
  }
};

struct MaterialDesc
{
  std::string_view Name;
  RGB Albedo{};
  float Roughness = 1.f;
  float Metallic = 0.f;
  RGB EmissionColor{};
  float EmissionPower = 0.f;
  std::optional<TextureSampler> AlbedoTexture = std::nullopt;
  std::optional<TextureSampler> MetallicRoughnessTexture = std::nullopt;
};

class Material
{
public:
  Material(MaterialDesc desc) : m_Name{desc.Name}, m_Albedo{desc.Albedo}, m_EmissionColor{desc.EmissionColor}, m_Metallic{desc.Metallic}, m_Roughness{desc.Roughness}, m_EmissionPower{desc.EmissionPower}, m_AlbedoTexture{std::move(desc.AlbedoTexture)}, m_MetallicRoughnessTexture{std::move(desc.MetallicRoughnessTexture)} {}

  const std::string& GetName() const
  {
    return m_Name;
  }

  constexpr RGB GetAlbedo() const
  {
    return m_Albedo;
  }

  RGB GetAlbedo(const Vec2& uv) const
  {
    if (m_AlbedoTexture.has_value())
    {
      return m_Albedo * m_AlbedoTexture->Sample(uv);
    }

    return m_Albedo;
  }

  bool HasAlbedoTexture() const
  {
    return m_AlbedoTexture.has_value();
  }

  constexpr RGB GetRadiance() const
  {
    return m_EmissionColor * m_EmissionPower;
  }

  constexpr float GetRoughness() const
  {
    return m_Roughness;
  }

  float GetRoughness(const Vec2& uv) const
  {
    if (m_MetallicRoughnessTexture.has_value())
    {
      return glm::clamp(m_Roughness * m_MetallicRoughnessTexture->Sample(uv).g, 0.0f, 1.0f);
    }

    return m_Roughness;
  }

  constexpr float GetMetallic() const
  {
    return m_Metallic;
  }

  float GetMetallic(const Vec2& uv) const
  {
    if (m_MetallicRoughnessTexture.has_value())
    {
      return glm::clamp(m_Metallic * m_MetallicRoughnessTexture->Sample(uv).b, 0.0f, 1.0f);
    }

    return m_Metallic;
  }

  bool HasMetallicRoughnessTexture() const
  {
    return m_MetallicRoughnessTexture.has_value();
  }

  float GetSpecularProbability() const
  {
    const RGB f0 = glm::mix(RGB{0.04f}, GetAlbedo(), GetMetallic());
    const float base_probability = std::max(f0.x, std::max(f0.y, f0.z));
    const float roughness_influence = glm::smoothstep(0.0f, 1.0f, GetRoughness() * 0.7f);
    return glm::mix(base_probability, base_probability * 0.5f, roughness_influence);
  }

  float GetSpecularProbability(const Vec2& uv) const
  {
    const RGB f0 = glm::mix(RGB{0.04f}, GetAlbedo(uv), GetMetallic(uv));
    const float base_probability = std::max(f0.x, std::max(f0.y, f0.z));
    const float roughness_influence = glm::smoothstep(0.0f, 1.0f, GetRoughness(uv) * 0.7f);
    return glm::mix(base_probability, base_probability * 0.5f, roughness_influence);
  }

  constexpr RGB GetEmissionColor() const
  {
    return m_EmissionColor;
  }
  constexpr float GetEmissionPower() const
  {
    return m_EmissionPower;
  }

private:
  std::string m_Name;
  RGB m_Albedo, m_EmissionColor;
  float m_Metallic;
  float m_Roughness;
  float m_EmissionPower{0.f};

  std::optional<TextureSampler> m_AlbedoTexture{std::nullopt};
  std::optional<TextureSampler> m_MetallicRoughnessTexture{std::nullopt};
};
} // namespace VI
