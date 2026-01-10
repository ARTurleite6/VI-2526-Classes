#pragma once

#include "Shaders/Shader.hpp"

namespace VI {
class DummyShader : public Shader {
public:
  RGB Execute(int x, int y, const Scene &scene, const Camera &camera) const {
    auto [width, height] = camera.GetResolution();

    return {x / width, y / height, 0.f};
  }
};
} // namespace VI
