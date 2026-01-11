#pragma once

namespace VI {

class Geometry;

struct Primitive {
  std::unique_ptr<Geometry> Geometry;
  int MaterialIndex;
};

} // namespace VI
