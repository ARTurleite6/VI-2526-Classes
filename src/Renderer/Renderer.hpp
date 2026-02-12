#pragma once

#include "Camera/Camera.hpp"
#include "Image/Image.hpp"
#include "Shaders/Shader.hpp"

namespace VI {
class Scene;
class Camera;

class Renderer final {
public:
  template <Shader S>
  Image Render(const Scene &scene [[maybe_unused]],
               const Camera &camera [[maybe_unused]],
               const S &shader [[maybe_unused]],
               int samples_per_pixel [[maybe_unused]] = 1,
               bool do_jittering [[maybe_unused]] = false) {}
};
} // namespace VI
