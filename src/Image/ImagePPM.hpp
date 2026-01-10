#pragma once

#include "Image/Image.hpp"

namespace VI {
class ImagePPM final : public Image {
public:
  ImagePPM(int width, int height) : Image{width, height} {}
  bool Load(std::string_view filename) const override;
  bool Save(std::string_view filename) const override;

private:
  void Clamp();
};
} // namespace VI
