#pragma once

#include "Camera/Camera.hpp"
#include "Image/Image.hpp"
#include "Shaders/Shader.hpp"
#include "Utils/ProgressBar.hpp"
#include "Utils/ThreadPool.hpp"

namespace VI {
class Scene;
class Camera;

constexpr float SAMPLES_PER_PIXEL = 64;

class Renderer final {
public:
  template <Shader S>
  Image Render(const Scene &scene, const Camera &camera, const S &shader) {
    auto [width, height] = camera.GetResolution();

    Image image{static_cast<int>(width), static_cast<int>(height)};
    ProgressBar progress{static_cast<int>(width * height)};

    ThreadPool pool{8};
    std::vector<std::future<void>> results;
    results.reserve(height);

    constexpr float spp_factor = 1.0f / SAMPLES_PER_PIXEL;
    for (int y = 0; y < static_cast<int>(height); ++y) {
      results.push_back(pool.Enqueue([&, y] {
        for (int x = 0; x < static_cast<int>(width); ++x) {
          RGB color = RGB{0.0f};
          for (int s = 0; s < SAMPLES_PER_PIXEL; s++) {
            color += shader.Execute(x, y, scene, camera);
          }
          image.Set(x, y, color * spp_factor);
          progress.Increment();
        }
      }));
    }

    for (auto &f : results) {
      f.get();
    }

    progress.Finish();

    return image;
  }
};
} // namespace VI
