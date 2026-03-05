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
  //PathTracingShader path_tracing_shader{{0.0f, 0.0f, 0.2f},
  //                                      DirectIlluminationMode::Uniform};
  PathTracingShader path_tracing_shader{{0.0f, 0.0f, 0.2f},
                                          DirectIlluminationMode::All};

  Scene scene = CreateCornellBox();

  int point_light_weak = scene.AddMaterial({.Name = "Point Light W",
                                            .EmissionColor = {1.f, 1.f, 1.f},
                                            .EmissionPower = 0.5f});

  scene.AddLight(std::make_unique<PointLight>(point_light_weak, Point{-70.f, 520.f, 30.f}));   // front-right
  scene.AddLight(std::make_unique<PointLight>(point_light_weak, Point{520.f, 520.f, 30.f}));   // front-left
  scene.AddLight(std::make_unique<PointLight>(point_light_weak, Point{-70.f, 520.f, 430.f}));  // back-right
  scene.AddLight(std::make_unique<PointLight>(point_light_weak, Point{520.f, 520.f, 430.f}));  // back-left

  scene.Build();
  Renderer renderer;
    const int spp=1;
  const auto image =
      renderer.Render(scene, camera, path_tracing_shader, spp, false);

  ImagePPM::Save(image, "image.ppm");

  auto end = std::chrono::system_clock::now();

  auto duration = std::chrono::duration<double>(end - begin);

  std::cout << "Time it took to render: " << duration.count() << " sec" << '\n';

  return 0;
}
