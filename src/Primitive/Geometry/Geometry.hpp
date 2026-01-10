#pragma once

#include "Ray/Intersection.hpp"
#include "Ray/Ray.hpp"

namespace VI {

class Geometry {
public:
  virtual ~Geometry() = default;

  virtual bool Intersect(const Ray &r, Intersection &i) const = 0;
};

} // namespace VI
