#include "ptmath.h"

Sphere::Sphere(const double radius, const vec3_t position) noexcept:
    m_radius{ radius },
    m_radiusSquared{ radius * radius },
    m_position{ position }
{}

std::optional<Primitive::intersection_t> Sphere::intersect(const ray_t& ray) const noexcept
{
    const auto op = m_position - ray.origin;
    const auto b = op.dot(ray.direction);
    const auto opSquared = op.lenSquared();
    const auto dSquared = std::pow(b, 2) - opSquared + m_radiusSquared;
    if(dSquared < 0) {
        return {};
    }

    const auto d = std::sqrt(dSquared);
    const auto inside = opSquared < m_radiusSquared;
    const auto t = inside ? b + d : b - d;
    if(t < ptmath_epsilon) {
        return {};
    }
    const auto position = ray.origin + ray.direction * t;
    const auto normal = (position - m_position) * (inside ? -1.0 : 1.0);

    return intersection_t{
        t,
        position,
        normal
    };
}

const double& Sphere::radius() const noexcept
{
    return m_radius;
}

const double& Sphere::radiusSquared() const noexcept
{
    return m_radiusSquared;
}

const vec3_t& Sphere::position() const noexcept
{
    return m_position;
}
