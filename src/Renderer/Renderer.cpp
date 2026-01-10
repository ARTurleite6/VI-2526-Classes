#include "Renderer/Renderer.hpp"

#include "Math/RGB.hpp"

namespace VI {

void Renderer::Render(const Scene &scene, const Camera &camera,
                      const Shader &shader, Image &image) {
  auto [width, height] = camera.GetResolution();
  for (int y = 0; y < width; ++y) {
    for (int x = 0; x < height; ++x) {

      RGB color = shader.Execute(x, y, scene, camera);

      image.Set(x, y, color);
    }
  }
}

} // namespace VI
