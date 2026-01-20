#pragma once

#include "Math/Vector.hpp"
#include "Ray/Ray.hpp"

namespace VI {
constexpr float MachineEpsilon = std::numeric_limits<float>::epsilon() * 0.5;

constexpr float gamma(int n) {
  return (n * MachineEpsilon) / (1 - n * MachineEpsilon);
}

struct BoundingBox {
  Point Min{std::numeric_limits<float>::max(),
            std::numeric_limits<float>::max(),
            std::numeric_limits<float>::max()};
  Point Max{std::numeric_limits<float>::lowest(),
            std::numeric_limits<float>::lowest(),
            std::numeric_limits<float>::lowest()};

  constexpr void Update(const Point &p) {
    if (p.x < Min.x) {
      Min.x = p.x;
    } else if (p.x > Max.x) {
      Max.x = p.x;
    }
    if (p.y < Min.y) {
      Min.y = p.y;
    } else if (p.y > Max.y) {
      Max.y = p.y;
    }
    if (p.z < Min.z) {
      Min.z = p.z;
    } else if (p.z > Max.z) {
      Max.z = p.z;
    }
  }

  constexpr bool Intersect(const Ray &ray, float &tmin, float &tmax) const {
    tmin = 0.f;
    tmax = std::numeric_limits<float>::max();

    // X slab
    float inv_ray_dir =
        (ray.Direction.x != 0.f ? 1.f / ray.Direction.x : 1.e5f);
    float t0x = (Min.x - ray.Origin.x) * inv_ray_dir;
    float t1x = (Max.x - ray.Origin.x) * inv_ray_dir;
    if (t0x > t1x)
      std::swap(t0x, t1x);
    t1x *= 1 + 2 * gamma(3);
    tmin = std::max(tmin, t0x);
    tmax = std::min(tmax, t1x);
    if (tmin > tmax)
      return false;

    // Y slab
    inv_ray_dir = (ray.Direction.y != 0.f ? 1.f / ray.Direction.y : 1.e5f);
    float t0y = (Min.y - ray.Origin.y) * inv_ray_dir;
    float t1y = (Max.y - ray.Origin.y) * inv_ray_dir;
    if (t0y > t1y)
      std::swap(t0y, t1y);
    t1y *= 1 + 2 * gamma(3);
    tmin = std::max(tmin, t0y);
    tmax = std::min(tmax, t1y);
    if (tmin > tmax)
      return false;

    // Z slab
    inv_ray_dir = (ray.Direction.z != 0.f ? 1.f / ray.Direction.z : 1.e5f);
    float t0z = (Min.z - ray.Origin.z) * inv_ray_dir;
    float t1z = (Max.z - ray.Origin.z) * inv_ray_dir;
    if (t0z > t1z)
      std::swap(t0z, t1z);
    t1z *= 1 + 2 * gamma(3);
    tmin = std::max(tmin, t0z);
    tmax = std::min(tmax, t1z);
    if (tmin > tmax)
      return false;

    return true;
  }

  constexpr void Update(const BoundingBox &other) {
    Min.x = glm::min(Min.x, other.Min.x);
    Min.y = glm::min(Min.y, other.Min.y);
    Min.z = glm::min(Min.z, other.Min.z);
    Max.x = glm::max(Max.x, other.Max.x);
    Max.y = glm::max(Max.y, other.Max.y);
    Max.z = glm::max(Max.z, other.Max.z);
  }
};
} // namespace VI
