#include "Shaders/AmbientShader.hpp"

namespace VI {
RGB AmbientShader::Execute(int x, int y, const Scene &scene,
                           const Camera &camera) const {
  Ray ray = camera.GenerateRay(x, y);

  Intersection intersection;
  if (!scene.Trace(ray, intersection)) {
    return m_BackgroundColor;
  }

  return {1.f, 0.0f, 0.0f};
}

} // namespace VI
