#ifndef PATHTRACER_RAY_H__
#define PATHTRACER_RAY_H__

struct vec3_t;
struct norm3_t;

struct ray_t
{
    vec3_t origin;
    norm3_t direction;
    
    ray_t(const vec3_t& origin, const norm3_t& direction) noexcept;
    static ray_t fromTwoPoints(const vec3_t from, const vec3_t& to) noexcept;
    ~ray_t() noexcept = default;
    
    ray_t(const ray_t& that) noexcept = default;
    ray_t(ray_t&& that) noexcept = default;
    
    ray_t& operator=(const ray_t& that) noexcept = default;
    ray_t& operator=(ray_t&& that) noexcept = default;
};

#endif // PATHTRACER_RAY_H__
