#include "Image/ImagePPM.hpp"

namespace VI {

void ImagePPM::Clamp() {}

bool ImagePPM::Save(std::string_view filename) const {
  std::ofstream file;
  file.open(filename.data(), std::ios::out); // Text mode is fine for P3

  if (file.fail()) {
    std::cout << "Failed to open file" << std::endl;
    return false;
  }

  file << "P3\n" << m_Width << " " << m_Height << "\n255\n";
  for (int i = 0; i < m_Width * m_Height; i++) {
    auto &data = m_Data[i];

    // Tone mapping (Reinhard)
    float r = data.r / (1.0f + data.r);
    float g = data.g / (1.0f + data.g);
    float b = data.b / (1.0f + data.b);

    // Gamma correction
    r = pow(r, 1.0f / 2.2f);
    g = pow(g, 1.0f / 2.2f);
    b = pow(b, 1.0f / 2.2f);

    // Clamp and convert to 8-bit
    int r8 = static_cast<int>(std::clamp(r, 0.0f, 1.0f) * 255.0f);
    int g8 = static_cast<int>(std::clamp(g, 0.0f, 1.0f) * 255.0f);
    int b8 = static_cast<int>(std::clamp(b, 0.0f, 1.0f) * 255.0f);

    file << r8 << " " << g8 << " " << b8 << "\n";
  }

  return true;
}

bool ImagePPM::Load(std::string_view filename) const {
  throw std::runtime_error("Not implemented");
}
} // namespace VI
