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

  constexpr int w = 670;
  constexpr int h = 550;

  constexpr Point Eye = {225, 282, -470};
  constexpr Point At = {225, 277, 0};

  constexpr Vector Up = {0, 1, 0};
  constexpr float fovH = 60.f;
  constexpr float fovHrad = fovH * 3.14f / 180.f; // to radians
  Camera camera{Eye, At, Up, w, h, fovHrad};
  //constexpr auto direct_mode = DirectIlluminationMode::Importance;
  constexpr auto direct_mode = DirectIlluminationMode::Uniform;
  //constexpr auto direct_mode = DirectIlluminationMode::All;
  PathTracingShader path_tracing_shader{{0.0f, 0.0f, 0.2f}, direct_mode};

  Scene scene = CreateImportanceSamplingCornellBox();

  scene.Build();
  Renderer renderer;
  // VERY LOW QUALITY
  constexpr int spp = 1;
  // LOW QUALITY
  //constexpr int spp = 16;
  // AVERAGE QUALITY
  //constexpr int spp = 64;
  // HIGH QUALITY
  //constexpr int spp = 512;
  const auto image =
      renderer.Render(scene, camera, path_tracing_shader, spp, false);

  ImagePPM::Save(image, "image.ppm");

  auto end = std::chrono::system_clock::now();

  auto duration = std::chrono::duration<double>(end - begin);

  std::cout << "Time it took to render: " << duration.count() << " sec" << '\n';

  return 0;
}
