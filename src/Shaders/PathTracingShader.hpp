#pragma once

#include "Shaders/Shader.hpp"

namespace VI {
struct Ray;
struct Intersection;

class PathTracingShader final {
public:
  RGB Execute(const Ray &ray, const Scene &scene) const;

private:
  RGB DoExecute(const Ray &ray, const Scene &scene,
                const Intersection &intersection, int depth = 0) const;

  RGB IndirectIllumination(const Ray &ray, const Scene &scene,
                           const Intersection &intersection, int depth) const;
};

static_assert(Shader<PathTracingShader>);
} // namespace VI
