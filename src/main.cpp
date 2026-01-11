#include "Camera/Camera.hpp"
#include "Image/FileImages.hpp"
#include "Math/Vector.hpp"
#include "Renderer/Renderer.hpp"
#include "Scene/Scene.hpp"
#include "Scene/SceneBuilder.hpp"
#include "Shaders/AmbientShader.hpp"
#include "Shaders/DummyShader.hpp"
#include "Shaders/WhittedShader.hpp"

using namespace VI;

int main() {
  constexpr int w = 800;
  constexpr int h = 600;

  constexpr Point Eye = {280, 265, -500};
  constexpr Point At = {280, 260, 0};

  constexpr Vector Up = {0, 1, 0};
  constexpr float fovH = 60.f;
  constexpr float fovHrad = fovH * 3.14f / 180.f; // to radians
  Camera camera{Eye, At, Up, w, h, fovHrad};
  Image image{800, 600};
  DummyShader shader;
  WhittedShader whitted_shader{{0.0f, 1.0, 0.0}};

  Scene scene = CreateCornellBox();

  // int ambient_light_material = scene.AddMaterial(
  //     {"Ambient Light", {0.f, 0.f, 0.f}, 1, {1.f, 1.f, 1.f}, 0.1f});
  // scene.AddLight(
  //     {.MaterialIndex = ambient_light_material, .Type = LightType::Ambient});

  int point_light = scene.AddMaterial(
      {"Point Light", {0.f, 0.f, 0.f}, 1, {1.f, 1.f, 1.f}, 1.0f});
  scene.AddLight({.Position = {200, 520.0, 200},
                  .MaterialIndex = point_light,
                  .Type = LightType::Point});

  Renderer renderer;
  renderer.Render(scene, camera, whitted_shader, image);

  ImagePPM::Save(image, "image.ppm");
  return 0;
}
