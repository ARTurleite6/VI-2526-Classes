#pragma once

#include "Shaders/Shader.hpp"

namespace VI {

class AmbientShader : public Shader {
public:
  AmbientShader(const RGB &background_color)
      : m_BackgroundColor(background_color) {}

  RGB Execute(int x, int y, const Scene &scene,
              const Camera &camera) const override;

private:
  RGB m_BackgroundColor;
};

} // namespace VI
