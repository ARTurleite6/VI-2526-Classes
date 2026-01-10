#pragma once

#include "Camera/Camera.hpp"
#include "Image/Image.hpp"
#include "Scene/Scene.hpp"
#include "Shaders/Shader.hpp"

namespace VI {
class Renderer {
public:
  void Render(const Scene &scene, const Camera &camera, const Shader &shader,
              Image &image);
};
} // namespace VI
