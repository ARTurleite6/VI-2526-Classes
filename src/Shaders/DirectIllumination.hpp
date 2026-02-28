#pragma once

#include "Math/RGB.hpp"

#include <optional>

namespace VI {

class Light;
class Material;
class Scene;
struct Intersection;
struct Ray;

struct SelectedLight {
  const Light *LightPtr;
  float SelectionPDF;
};

std::optional<SelectedLight> SampleLightUniform(const Scene &scene);

RGB EstimateDirectIllumination(const Ray &ray, const Scene &scene,
                               const Intersection &intersection,
                               const Material &material,
                               const SelectedLight &selected_light);

RGB SampleDirectIlluminationUniform(const Ray &ray, const Scene &scene,
                                    const Intersection &intersection,
                                    const Material &material);

} // namespace VI
