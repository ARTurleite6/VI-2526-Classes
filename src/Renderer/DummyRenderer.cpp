#include "Renderer/DummyRenderer.hpp"

#include "Camera/Camera.hpp"
#include "Shaders/DummyShader.hpp"
#include "Utils/ProgressBar.hpp"

namespace VI {

Image DummyRenderer::Render(const Scene &scene, const Camera &camera) {
  auto [width, height] = camera.GetResolution();
  DummyShader shader{static_cast<int>(width), static_cast<int>(height)};

  ProgressBar bar{static_cast<int>(width * height)};
  Image image{static_cast<int>(width), static_cast<int>(height)};

  for (int y = 0; y < height; ++y) {
    for (int x = 0; x < width; ++x) {
      Ray ray = camera.GenerateRay(x, y);
      ray.Direction.x = x;
      ray.Direction.y = y;

      RGB color = shader.Execute(ray, scene);
      image.Set(x, y, color);
      bar.Increment();
    }
  }

  bar.Finish();

  return image;
}

} // namespace VI
