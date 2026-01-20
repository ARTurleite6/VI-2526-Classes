#pragma once

#include "Camera/Camera.hpp"
#include "Scene/Scene.hpp"
#include "Shaders/Shader.hpp"

namespace VI {
class DummyShader final {
public:
  RGB Execute(int x, int y, const Scene &scene [[maybe_unused]],
              const Camera &camera) const {
    auto [width, height] = camera.GetResolution();

    return {x / width, y / height, 0.f};
  }
};
} // namespace VI
