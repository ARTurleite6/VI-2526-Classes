#include "Camera/Camera.hpp"
#include "Image/FileImages.hpp"
#include "Math/Vector.hpp"
#include "Renderer/Renderer.hpp"
#include "Scene/Scene.hpp"
#include "Scene/SceneBuilder.hpp"
#include "Shaders/AmbientShader.hpp"
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
  WhittedShader whitted_shader{{0.0f, 0.0f, 1.0f}};

  Scene scene = WhittedCornellBox();

  int point_light = scene.AddMaterial(
      {.Name = "Point Light", .EmissionColor = {1.f, 1.f, 1.f}, .EmissionPower = 2.f});
  // Center light
  scene.AddLight(
      std::make_unique<PointLight>(point_light, Point{280, 540.f, 230}));
  // Corner lights (near ceiling, slightly inward from each vertical corner)
  scene.AddLight(std::make_unique<PointLight>(point_light, Point{-70.f, 520.f, 30.f}));   // front-right
  scene.AddLight(std::make_unique<PointLight>(point_light, Point{520.f, 520.f, 30.f}));   // front-left
  scene.AddLight(std::make_unique<PointLight>(point_light, Point{-70.f, 520.f, 430.f}));  // back-right
  scene.AddLight(std::make_unique<PointLight>(point_light, Point{520.f, 520.f, 430.f}));  // back-left

  int ambient_mat = scene.AddMaterial(
      {.Name = "Ambient Light", .EmissionColor = {1.f, 1.f, 1.f}, .EmissionPower = 0.05f});
  scene.AddLight(std::make_unique<AmbientLight>(ambient_mat));

  scene.Build();
  Renderer renderer;
  const auto image =
      renderer.Render(scene, camera, whitted_shader, 1);

  ImagePPM::Save(image, "image.ppm");

  auto end = std::chrono::system_clock::now();

  auto duration = std::chrono::duration<double>(end - begin);

  std::cout << "Time it took to render: " << duration.count() << " sec" << '\n';

  return 0;
}
