#pragma once

#include "Math/RGB.hpp"
#include "Shaders/Shader.hpp"

namespace VI {
struct Ray;
struct Intersection;

class PathTracingShader final {
public:
  explicit PathTracingShader(const RGB &background_color = {})
      : m_BackgroundColor{background_color} {}

  RGB Execute(const Ray &ray, const Scene &scene) const;

private:
  RGB DoExecute(const Ray &ray, const Scene &scene,
                const Intersection &intersection, int depth = 0,
                bool allow_emissive = true) const;

  RGB DirectIllumination(const Ray &ray, const Scene &scene,
                         const Intersection &intersection) const;

  RGB IndirectIllumination(const Ray &ray, const Scene &scene,
                           const Intersection &intersection, int depth,
                           bool allow_emissive) const;

  RGB m_BackgroundColor;
};

static_assert(Shader<PathTracingShader>);
} // namespace VI
