#pragma once

namespace VI {
class Scene;
class Camera;
class Image;
class Shader;

class Renderer final {
public:
  void Render(const Scene &scene, const Camera &camera, const Shader &shader,
              Image &image);
};
} // namespace VI
