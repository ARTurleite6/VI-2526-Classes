#pragma once

#include "Math/RGB.hpp"
#include "Math/Vector.hpp"

namespace VI {

class Material;

class BRDF {
public:
  virtual ~BRDF() = default;

  virtual RGB Sample(const Vector &wo_local,
                     const Material &material) const = 0;
  virtual Vector Evaluate(const Vector &wo_local, const Vector &wi_local,
                          const Material &material) const = 0;
  virtual float PDF(const Vector &wo_local, const Vector &wi_local,
                    const Material &material) const = 0;
};

} // namespace VI
