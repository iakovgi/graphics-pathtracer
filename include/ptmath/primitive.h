#ifndef PATHTRACER_PRIMITIVE_H__
#define PATHTRACER_PRIMITIVE_H__

struct ray_t;
struct vec3_t;
struct norm3_t;

#include <optional>

class Primitive
{
public:
    struct intersection_t
    {
        double distance;
        vec3_t position;
        norm3_t normal;
    };
    
    virtual std::optional<intersection_t> intersect(const ray_t& ray) const noexcept = 0;
    
    virtual ~Primitive() noexcept = default;
};

class SpherePrimitive final: public Primitive
{
public:
    virtual std::optional<intersection_t> intersect(const ray_t& ray) const noexcept override;

    SpherePrimitive(const double radius, const vec3_t position) noexcept;

    const double& radius() const noexcept;
    const double& radiusSquared() const noexcept;
    const vec3_t& position() const noexcept;

    virtual ~SpherePrimitive() noexcept override = default;

    SpherePrimitive(const SpherePrimitive& that) noexcept = default;
    SpherePrimitive(SpherePrimitive&& that) noexcept = default;

    SpherePrimitive& operator=(const SpherePrimitive& that) noexcept = default;
    SpherePrimitive& operator=(SpherePrimitive&& that) noexcept = default;

private:
    double m_radius;
    double m_radiusSquared;
    vec3_t m_position;
};

class TrianglePrimitive final: public Primitive
{
public:
    virtual std::optional<intersection_t> intersect(const ray_t& ray) const noexcept override;

    TrianglePrimitive(const vec3_t& v0, const vec3_t& v1, const vec3_t& v2) noexcept;

    const norm3_t& norm() const noexcept;
    const vec3_t& v0() const noexcept;
    const vec3_t& v1() const noexcept;
    const vec3_t& v2() const noexcept;

    virtual ~TrianglePrimitive() noexcept override = default;

    TrianglePrimitive(const TrianglePrimitive& that) noexcept = default;
    TrianglePrimitive(TrianglePrimitive&& that) noexcept = default;

    TrianglePrimitive& operator=(const TrianglePrimitive& that) noexcept = default;
    TrianglePrimitive& operator=(TrianglePrimitive&& that) noexcept = default;

private:
    vec3_t m_v0;
    vec3_t m_v1;
    vec3_t m_v2;

    vec3_t m_v01;
    vec3_t m_v02;

    norm3_t m_norm;
};

#endif // PATHTRACER_PRIMITIVE_H__
