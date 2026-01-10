#pragma once

#include "Math/RGB.hpp"

namespace VI {
class Image {
public:
  Image(int width, int height)
      : m_Width{width}, m_Height{height},
        m_Data{static_cast<size_t>(width * height)} {
    if (m_Width <= 0 || m_Height <= 0) {
      throw std::invalid_argument("Invalid image dimensions");
    }
  }
  virtual ~Image() = default;

  inline void Set(int x, int y, const RGB &rgb) {
    m_Data[x + y * m_Width] = rgb;
  }
  inline void Add(int x, int y, const RGB &rgb) {
    m_Data[x + y * m_Width] += rgb;
  }

  virtual bool Save(std::string_view filename) const = 0;
  virtual bool Load(std::string_view filename) const = 0;

protected:
  std::vector<RGB> m_Data{};
  int m_Width, m_Height;
};
} // namespace VI
