#pragma once

#include "Math/RGB.hpp"

namespace VI {
class Camera;
class Scene;

template <class T>
concept Shader = requires(const T &shader, int x, int y, const Scene &scene,
                          const Camera &camera) {
  { shader.Execute(x, y, scene, camera) } -> std::same_as<RGB>;
};

// class Shader {
// public:
//   virtual ~Shader() = default;

//   virtual RGB Execute(int x, int y, const Scene &scene,
//                       const Camera &camera) const = 0;
// };
} // namespace VI
