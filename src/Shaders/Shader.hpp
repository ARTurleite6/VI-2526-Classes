#pragma once

#include "Camera/Camera.hpp"
#include "Math/RGB.hpp"
#include "Scene/Scene.hpp"

namespace VI {
class Shader {
public:
  virtual ~Shader() = default;

  virtual RGB Execute(int x, int y, const Scene &scene,
                      const Camera &camera) const = 0;
};
} // namespace VI
