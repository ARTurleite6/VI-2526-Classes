#include "Shaders/PathTracingShader.hpp"

#include "Math/RGB.hpp"
#include "Ray/Intersection.hpp"
#include "Scene/Scene.hpp"

namespace VI {
RGB PathTracingShader::Execute(const Ray &ray [[maybe_unused]],
                               [[maybe_unused]] const Scene &scene
                               [[maybe_unused]]) const {
  return {};
}

RGB PathTracingShader::DoExecute(const Ray &ray [[maybe_unused]],
                                 const Scene &scene [[maybe_unused]],
                                 const Intersection &intersection
                                 [[maybe_unused]],
                                 int depth [[maybe_unused]]) const {
  return {};
}

RGB PathTracingShader::IndirectIllumination(const Ray &ray [[maybe_unused]],
                                            const Scene &scene [[maybe_unused]],
                                            const Intersection &intersection
                                            [[maybe_unused]],
                                            int depth [[maybe_unused]]) const {
  return {};
}

} // namespace VI
