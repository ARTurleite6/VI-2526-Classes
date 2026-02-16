#include "Camera/Camera.hpp"
#include "Image/FileImages.hpp"
#include "Math/Vector.hpp"
#include "Renderer/Renderer.hpp"
#include "Scene/Scene.hpp"
#include "Scene/SceneBuilder.hpp"
#include "Shaders/AmbientShader.hpp"
#include "Shaders/PathTracingShader.hpp"
#include "Shaders/WhittedShader.hpp"

using namespace VI;

int main() {
  auto begin = std::chrono::system_clock::now();

  constexpr int w = 800;
  constexpr int h = 600;

  constexpr Point Eye = {280, 265, -500};
  constexpr Point At = {280, 260, 0};

  constexpr Vector Up = {0, 1, 0};
  constexpr float fovH = 60.f;
  constexpr float fovHrad = fovH * 3.14f / 180.f; // to radians
  Camera camera{Eye, At, Up, w, h, fovHrad};
  PathTracingShader pathtracing_shader{};

  Scene scene = CreateCornellBox();

  int point_light = scene.AddMaterial(
      {"Point Light", {0.f, 0.f, 0.f}, 1, 0, {1.f, 1.f, 1.f}, 10.0f});
  scene.AddLight(
      std::make_unique<PointLight>(point_light, Point{200, 520.0, 200}));

  scene.Build();
  Renderer renderer;
  const auto image =
      renderer.Render(scene, camera, pathtracing_shader, 256, true);

  ImagePPM::Save(image, "image.ppm");

  auto end = std::chrono::system_clock::now();

  auto duration = std::chrono::duration<double>(end - begin);

  std::cout << "Time it took to render: " << duration.count() << " sec" << '\n';

  return 0;
}
