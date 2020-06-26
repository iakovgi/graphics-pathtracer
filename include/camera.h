#ifndef PATHTRACER_CAMERA_H__
#define PATHTRACER_CAMERA_H__

#include "ptmath.h"

class Camera
{
public:
    Camera(const vec3_t& eye,
           const norm3_t& gaze,
           const double vertFoV = 60.0,
           const vec3_t& up = OrthoNormalBasis::worldY()) noexcept;
    
    Camera() noexcept;
    ~Camera() noexcept = default;
    
    void setViewPortSize(const size_t width, const size_t height) const noexcept;
    ray_t randomRay(Random& rng, size_t px, size_t py) const noexcept;
    
    Camera(const Camera& that) noexcept = default;
    Camera(Camera&& that) noexcept = default;
    
    Camera& operator=(const Camera& that) noexcept = default;
    Camera& operator=(Camera&& that) noexcept = default;
    
private:
    mutable size_t m_width;
    mutable size_t m_height;
    mutable double m_dp;
    
    vec3_t m_eye;
    OrthoNormalBasis m_onb;

    double m_fov;
};

#endif // PATHTRACER_CAMERA_H__
