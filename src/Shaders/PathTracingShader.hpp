#pragma once

#include "Shaders/Shader.hpp"

namespace VI {
class PathTracingShader final : public Shader {
  RGB Execute(int x, int y, const Scene &scene,
              const Camera &camera) const override;
};
} // namespace VI
