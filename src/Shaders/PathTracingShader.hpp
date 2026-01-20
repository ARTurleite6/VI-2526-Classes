#pragma once

#include "Shaders/Shader.hpp"

namespace VI {
struct Ray;
struct Intersection;

class PathTracingShader final {
public:
  RGB Execute(int x, int y, const Scene &scene, const Camera &camera) const;

private:
  RGB DoExecute(const Ray &ray, const Scene &scene, const Camera &camera,
                const Intersection &intersection, int depth = 0) const;

  RGB IndirectIllumination(const Ray &ray, const Scene &scene,
                           const Camera &camera,
                           const Intersection &intersection, int depth) const;
};

static_assert(Shader<PathTracingShader>);
} // namespace VI
