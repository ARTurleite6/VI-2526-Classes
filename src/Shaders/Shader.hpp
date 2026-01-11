#pragma once

#include "Math/RGB.hpp"

namespace VI {
class Camera;
class Scene;

class Shader {
public:
  virtual ~Shader() = default;

  virtual RGB Execute(int x, int y, const Scene &scene,
                      const Camera &camera) const = 0;
};
} // namespace VI
