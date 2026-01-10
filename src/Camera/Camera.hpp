#pragma once

#include "Math/Vector.hpp"
#include "Ray/Ray.hpp"

namespace VI {
class Camera {
public:
  struct Resolution {
    float Width, Height;
  };

  Camera(Point eye, Point at, Vector up, int width, int height, float fov_h,
         float defocus_angle = 0.f, float focus_dist = 1.);

  Ray GenerateRay(int x, int y, glm::vec2 jitter = {0.5f, 0.5f}) const;

  inline Resolution GetResolution() const {
    return {static_cast<float>(m_Width), static_cast<float>(m_Height)};
  }

private:
  Point m_Eye, m_At;
  Vector m_Up;
  int m_Width, m_Height;

  Point m_Pixel00Location;
  Vector m_PixelDeltaU, m_PixelDeltaV;
  Vector m_DefocusDiskRight, m_DefocusDiskUp;
  float m_DefocusAngle;
};
} // namespace VI
