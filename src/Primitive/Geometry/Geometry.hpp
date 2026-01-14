#pragma once

#include "Mesh.hpp"
#include "Sphere.hpp"
#include "Triangle.hpp"

namespace VI {

class Ray;
class Intersection;

// TODO: check if this should be a variant or if we should just have an
// array for each type of shape on the Scene.
using Geometry = std::variant<Sphere, Triangle, Mesh>;

bool Intersect(const Geometry &geom, const Ray &ray,
               Intersection &intersection);

} // namespace VI
