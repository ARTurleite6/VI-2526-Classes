#include "Shaders/AmbientShader.hpp"

#include "Math/RGB.hpp"
#include "Scene/Scene.hpp"

namespace VI {
RGB AmbientShader::Execute(const Ray &ray [[maybe_unused]],
                           const Scene &scene [[maybe_unused]]) const {
  return {};
}

} // namespace VI
