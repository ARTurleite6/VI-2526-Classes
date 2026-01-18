#include "Renderer/Renderer.hpp"

#include "Camera/Camera.hpp"
#include "Math/RGB.hpp"
#include "Scene/Scene.hpp"
#include "Shaders/Shader.hpp"

namespace VI {

Image Renderer::Render(const Scene &scene, const Camera &camera,
                      const Shader &shader) {
  auto [width, height] = camera.GetResolution();

  Image image{static_cast<int>(width), static_cast<int>(height)};
  
  for (int y = 0; y < static_cast<int>(height); ++y) {
    for (int x = 0; x < static_cast<int>(width); ++x) {

      RGB color = shader.Execute(x, y, scene, camera);

      image.Set(x, y, color);
    }
  }

  return image;
}

} // namespace VI
