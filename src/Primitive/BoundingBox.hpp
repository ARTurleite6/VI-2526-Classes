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

  constexpr bool Intersect(const Ray &ray) const {
    float t0 = 0.f, t1 = std::numeric_limits<float>::max();
    float inv_ray_dir, t_near, t_far;
    // XX slabs
    inv_ray_dir = (ray.Direction.x != 0.f ? 1.f / ray.Direction.x : 1.e5);
    // invRayDir = r.invDir.X;
    t_near = (Min.x - ray.Origin.x) * inv_ray_dir;
    t_far = (Max.x - ray.Origin.x) * inv_ray_dir;
    if (t_near > t_far) {
      float aux = t_near;
      t_near = t_far;
      t_far = aux;
    }
    t_far *= 1 + 2 * gamma(3);
    t0 = t_near > t0 ? t_near : t0;
    t1 = t_far < t1 ? t_far : t1;
    if (t0 > t1)
      return false;
    // YY slabs
    // invRayDir = 1 / r.dir.Y;
    inv_ray_dir = (ray.Direction.y != 0.f ? 1.f / ray.Direction.y : 1.e5);
    // invRayDir = r.invDir.Y;
    t_near = (Min.y - ray.Origin.y) * inv_ray_dir;
    t_far = (Max.y - ray.Origin.y) * inv_ray_dir;
    if (t_near > t_far) {
      float aux = t_near;
      t_near = t_far;
      t_far = aux;
    }
    t_far *= 1 + 2 * gamma(3);
    t0 = t_near > t0 ? t_near : t0;
    t1 = t_far < t1 ? t_far : t1;
    if (t0 > t1)
      return false;
    // ZZ slabs
    // invRayDir = 1 / r.dir.Z;
    inv_ray_dir = (ray.Direction.z != 0.f ? 1.f / ray.Direction.z : 1.e5);
    // invRayDir = r.invDir.Z;
    t_near = (Min.z - ray.Origin.z) * inv_ray_dir;
    t_far = (Max.z - ray.Origin.z) * inv_ray_dir;
    if (t_near > t_far) {
      float aux = t_near;
      t_near = t_far;
      t_far = aux;
    }
    t_far *= 1 + 2 * gamma(3);
    t0 = t_near > t0 ? t_near : t0;
    t1 = t_far < t1 ? t_far : t1;
    if (t0 > t1)
      return false;

    return true;
  }
};
} // namespace VI
