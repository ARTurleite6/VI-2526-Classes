#include "Camera/Camera.hpp"
#include "Image/FileImages.hpp"
#include "Math/Vector.hpp"
#include "Renderer/Renderer.hpp"
#include "Scene/Scene.hpp"
#include "Scene/SceneBuilder.hpp"
#include "Shaders/AmbientShader.hpp"
#include "Shaders/DirectIllumination.hpp"
#include "Shaders/PathTracingShader.hpp"
#include "Shaders/WhittedShader.hpp"

#include <chrono>
#include <iostream>

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
  constexpr auto direct_mode = DirectIlluminationMode::Importance;
  // constexpr auto direct_mode = DirectIlluminationMode::Uniform;
  PathTracingShader path_tracing_shader{{0.0f, 0.0f, 0.2f}, direct_mode};

  Scene scene = CreateImportanceSamplingCornellBox();

  scene.Build();
  Renderer renderer;
  constexpr int spp = 40;
  const auto image =
      renderer.Render(scene, camera, path_tracing_shader, spp, false);

  ImagePPM::Save(image, "image.ppm");

  auto end = std::chrono::system_clock::now();

  auto duration = std::chrono::duration<double>(end - begin);

  std::cout << "Time it took to render: " << duration.count() << " sec" << '\n';

  return 0;
}
