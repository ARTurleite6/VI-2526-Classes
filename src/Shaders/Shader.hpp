#pragma once

#include "Math/RGB.hpp"

namespace VI {
class Camera;
class Scene;
struct Ray;

template <class T>
concept Shader = requires(const T &shader, const Ray &ray, const Scene &scene) {
  { shader.Execute(ray, scene) } -> std::same_as<RGB>;
};

// class Shader {
// public:
//   virtual ~Shader() = default;

//   virtual RGB Execute(int x, int y, const Scene &scene,
//                       const Camera &camera) const = 0;
// };
} // namespace VI
