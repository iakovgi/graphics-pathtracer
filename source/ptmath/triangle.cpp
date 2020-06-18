#include "ptmath.h"

Triangle::Triangle(const vec3_t& v0, const vec3_t& v1, const vec3_t& v2) noexcept:
    m_v0{ v0 },
    m_v1{ v1 },
    m_v2{ v2 },
    m_v01{ v1 - v0 },
    m_v02{ v2 - v0 },
    m_orientation{ norm3_t((v1 - v0).cross(v2 - v0)) }
{}

std::optional<Primitive::intersection_t> Triangle::intersect(const ray_t& ray) const noexcept
{
    const auto pVec = ray.direction.cross(m_v02);
    const auto det = m_v01.dot(pVec);
    
    if (std::abs(det) < PTMATH_EPSILON) {
        return {};
    }

    const auto backfacing = det < PTMATH_EPSILON;

    const auto invDet = 1.0 / det;
    const auto tVec = ray.origin - m_v0;
    const auto u = tVec.dot(pVec) * invDet;

    const auto qVec = tVec.cross(m_v01);
    const auto v = ray.direction.dot(qVec) * invDet;

    // extra parens to keep clang-format happy...
    if(u < 0.0 || u > 1.0 || v < 0.0 || u + v > 1.0) {
        return {};
    }

    const auto t = m_v02.dot(qVec) * invDet;
    if(t < PTMATH_EPSILON) {
        return {};
    }

    const auto normal = backfacing ? (-1.0 * m_orientation).norm() : m_orientation;
    return intersection_t{
        t,
        ray.origin + ray.direction * t,
        normal
    };
}

const norm3_t& Triangle::orientation() const noexcept
{
    return m_orientation;
}
