#include "ptmath.h"

// SpherePrimitive -----------------------------------------------------------------------------------------------------

SpherePrimitive::SpherePrimitive(const double radius, const vec3_t position) noexcept:
        m_radius{ radius },
        m_radiusSquared{ radius * radius },
        m_position{ position }
{}

std::optional<Primitive::intersection_t> SpherePrimitive::intersect(const ray_t& ray) const noexcept
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
    if(t < PTMATH_EPSILON) {
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

const double& SpherePrimitive::radius() const noexcept
{
    return m_radius;
}

const double& SpherePrimitive::radiusSquared() const noexcept
{
    return m_radiusSquared;
}

const vec3_t& SpherePrimitive::position() const noexcept
{
    return m_position;
}

// ---------------------------------------------------------------------------------------------------------------------

// TrianglePrimitive ---------------------------------------------------------------------------------------------------

TrianglePrimitive::TrianglePrimitive(const vec3_t& v0, const vec3_t& v1, const vec3_t& v2) noexcept:
        m_v0{ v0 },
        m_v1{ v1 },
        m_v2{ v2 },
        m_v01{ v1 - v0 },
        m_v02{ v2 - v0 },
        m_norm{ norm3_t((v1 - v0).cross(v2 - v0)) }
{}

std::optional<Primitive::intersection_t> TrianglePrimitive::intersect(const ray_t& ray) const noexcept
{
    const auto norm = ray.direction.dot(m_norm) < 0.0 ? m_norm : norm3_t(-1.0 * m_norm);
    const auto dn = ray.direction.dot(norm);
    if(std::abs(dn) <= PTMATH_EPSILON) {
        return {};
    }

    const auto t = (m_v0 - ray.origin).dot(norm) / dn;
    if(t <= PTMATH_EPSILON) {
        return {};
    }

    const auto p = ray.origin + ray.direction * t;

    const auto p1 = p - m_v0;
    const auto v12 = m_v2 - m_v1;
    const auto posDir = m_norm;
    const auto m = m_v01.cross(p1).dot(posDir);
    const auto n = p1.cross(m_v02).dot(posDir);
    const auto k = v12.cross(p - m_v1).dot(posDir);
    if(m <= 0.0 || n <= 0.0 || k <= 0.0) {
        return {};
    }
    return intersection_t{
        .distance = t,
        .position = p,
        .normal = norm
    };
}

const norm3_t& TrianglePrimitive::norm() const noexcept
{
    return m_norm;
}

const vec3_t& TrianglePrimitive::v0() const noexcept
{
    return m_v0;
}

const vec3_t& TrianglePrimitive::v1() const noexcept
{
    return m_v1;
}

const vec3_t& TrianglePrimitive::v2() const noexcept
{
    return m_v2;
}

// ---------------------------------------------------------------------------------------------------------------------
