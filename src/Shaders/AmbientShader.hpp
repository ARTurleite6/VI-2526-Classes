#pragma once

#include "Math/RGB.hpp"
#include "Shaders/Shader.hpp"

namespace VI {

class Scene;
class Camera;

class AmbientShader final {
public:
  AmbientShader(const RGB &background_color)
      : m_BackgroundColor(background_color) {}

  RGB Execute(int x, int y, const Scene &scene, const Camera &camera) const;

private:
  RGB m_BackgroundColor;
};

static_assert(Shader<AmbientShader>);

} // namespace VI
