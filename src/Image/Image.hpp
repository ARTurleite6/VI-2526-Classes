#pragma once

#include "Math/RGB.hpp"

namespace VI {
class Image final {
public:
  constexpr Image(int width, int height)
      : m_Width{width}, m_Height{height},
        m_Data{static_cast<size_t>(width * height)} {
    if (m_Width <= 0 || m_Height <= 0) {
      throw std::invalid_argument("Invalid image dimensions");
    }
  }

  constexpr int GetWidth() const { return m_Width; }
  constexpr int GetHeight() const { return m_Height; }

  constexpr void Set(int x, int y, const RGB &rgb) {
    m_Data[x + y * m_Width] = rgb;
  }

  constexpr const RGB &Get(int x, int y) const {
    return m_Data[x + y * m_Width];
  }

  constexpr void Add(int x, int y, const RGB &rgb) {
    m_Data[x + y * m_Width] += rgb;
  }

private:
  std::vector<RGB> m_Data{};
  int m_Width, m_Height;
};
} // namespace VI
