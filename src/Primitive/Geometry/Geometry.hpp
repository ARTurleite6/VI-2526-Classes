#pragma once

namespace VI {

class Ray;
class Intersection;

class Geometry {
public:
  virtual ~Geometry() = default;

  virtual bool Intersect(const Ray &r, Intersection &i) const = 0;
};

} // namespace VI
