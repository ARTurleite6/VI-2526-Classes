#include <memory>

#include "Camera/Camera.hpp"
#include "Image/ImagePPM.hpp"
#include "Math/Vector.hpp"
#include "Primitive/Geometry/Sphere.hpp"
#include "Renderer/Renderer.hpp"
#include "Scene/Scene.hpp"
#include "Shaders/AmbientShader.hpp"
#include "Shaders/DummyShader.hpp"

using namespace VI;

int main() {
  constexpr int w = 800;
  constexpr int h = 600;

  constexpr Point Eye = {280, 260, -10};
  constexpr Point At = {280, 260, 3};

  constexpr Vector Up = {0, 1, 0};
  constexpr float fovH = 90.f;
  constexpr float fovHrad = fovH * 3.14f / 180.f; // to radians
  Camera camera{Eye, At, Up, w, h, fovHrad};
  ImagePPM image{800, 600};
  DummyShader shader;
  AmbientShader ambient_shader{{0.0f, 0.0, 0.2}};

  Scene scene;
  // Center sphere on screen: place at the look-at point (At)
  scene.AddPrimitive(std::make_shared<Sphere>(At, 0.8));

  Renderer renderer;
  renderer.Render(scene, camera, ambient_shader, image);

  image.Save("image.ppm");
  return 0;
}
