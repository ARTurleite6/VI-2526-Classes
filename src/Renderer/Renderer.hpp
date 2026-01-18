#pragma once

#include "Image/Image.hpp"

namespace VI {
class Scene;
class Camera;
class Shader;

class Renderer final {
public:
  Image Render(const Scene &scene, const Camera &camera, const Shader &shader);
};
} // namespace VI
