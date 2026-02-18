#include "Shaders/WhittedShader.hpp"

#include "Math/RGB.hpp"
#include "Ray/Intersection.hpp"
#include "Ray/Ray.hpp"
#include "Scene/Scene.hpp"

namespace VI {

RGB WhittedShader::Execute(const Ray &ray [[maybe_unused]],
                           const Scene &scene [[maybe_unused]]) const {
  return {};
}

RGB WhittedShader::DoExecute(const Ray &ray [[maybe_unused]],
                             const Scene &scene [[maybe_unused]],
                             const Intersection &intersection [[maybe_unused]],
                             int depth [[maybe_unused]]) const {
  return {};
}

RGB WhittedShader::DirectIllumination(const Ray &ray, const Scene &scene,
                         const Intersection &intersection) const {
  return {};
}

RGB WhittedShader::IndirectIllumination(const Ray &ray [[maybe_unused]],
                                        const Scene &scene [[maybe_unused]],
                                        const Intersection &intersection
                                        [[maybe_unused]],
                                        int depth [[maybe_unused]]) const {
  return {};
}
} // namespace VI
