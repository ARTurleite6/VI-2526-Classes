#pragma once

#include "Math/RGB.hpp"
#include "Shaders/Shader.hpp"

namespace VI {

class Scene;
class Camera;

class WhittedShader final : public Shader {
public:
  WhittedShader(const RGB &background_color)
      : m_BackgroundColor(background_color) {}

  RGB Execute(int x, int y, const Scene &scene,
              const Camera &camera) const override;

private:
  RGB m_BackgroundColor;
};

} // namespace VI
