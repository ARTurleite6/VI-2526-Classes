#pragma once

#include "Image/Image.hpp"

namespace VI {
class Scene;
class Camera;

class DummyRenderer final {
public:
  Image Render(const Scene &scene, const Camera &camera);
};
} // namespace VI
