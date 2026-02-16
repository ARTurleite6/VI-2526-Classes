#pragma once

#include "Scene/Scene.hpp"
#include "Shaders/Shader.hpp"

namespace VI {
struct Ray;

class DummyShader final {
public:
  DummyShader(int width, int height) : m_Width{width}, m_Height{height} {}

  RGB Execute(const Ray &ray [[maybe_unused]],
              const Scene &scene [[maybe_unused]]) const {
    return {ray.Direction.x / m_Width, ray.Direction.y / m_Height, 0.f};
  }

private:
  int m_Width, m_Height;
};

static_assert(Shader<DummyShader>);
} // namespace VI
