#pragma once

#include "Primitive/Geometry/Geometry.hpp"

namespace VI {

struct Primitive {
  std::shared_ptr<Geometry> Geometry;
  int MaterialIndex;
};

} // namespace VI
