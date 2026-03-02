#pragma once

#include "Math/RGB.hpp"

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

enum class DirectIlluminationMode {
  Uniform,
  All,
};

RGB EstimateDirectIllumination(const Ray &ray, const Scene &scene,
                               const Intersection &intersection,
                               const Material &material,
                               const SelectedLight &selected_light);

RGB SampleDirectIllumination(
    const Ray &ray, const Scene &scene, const Intersection &intersection,
    const Material &material,
    DirectIlluminationMode mode = DirectIlluminationMode::Uniform);

} // namespace VI
