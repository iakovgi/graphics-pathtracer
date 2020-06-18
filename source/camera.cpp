#include "camera.h"

Camera::Camera(const vec3_t& eye, const norm3_t& gaze, const double vertFoV, const vec3_t& up) noexcept:
    m_width{ 1 },
    m_height{ 1 },
    m_eye{ eye },
    m_fov{ vertFoV / 180.0 * M_PI }
{
    m_dp = 2.0 * std::tan(m_fov / 2.0) / m_height;
    
    const norm3_t z = gaze;
    const norm3_t x = gaze.cross(up);
    const norm3_t y = x.cross(z);
    
    m_onb = ONB(x, y, z);
}

Camera::Camera() noexcept
{
    new(this) Camera(vec3_t{ 0.0, 0.0, 0.0 }, vec3_t{ 0.0, 0.0, 1.0 });
}

void Camera::setViewPortSize(const size_t width, const size_t height) const noexcept
{
    m_width = width;
    m_height = height;
    m_dp = 2.0 * std::tan(m_fov / 2.0) / m_height;
}

ray_t Camera::randomRay(Random& rng, size_t px, size_t py) const noexcept
{
    const auto dq = rng.getUniform();
    const auto dp = rng.getUniform();

    const auto q = (px - m_width  / 2.0 + dq) * m_dp;
    const auto p = (py - m_height / 2.0 + dp) * m_dp;

    const auto local = vec3_t{ q, p, 1.0 };
    
    return ray_t(m_eye, m_onb.toWorld(local.norm()));
}
