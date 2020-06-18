#include "ptmath.h"

ONB::ONB() noexcept:
    m_x{ ONB::xAxis() },
    m_y{ ONB::yAxis() },
    m_z{ ONB::zAxis() }
{}

ONB::ONB(const norm3_t& x, const norm3_t& y, const norm3_t& z) noexcept:
    m_x{ x },
    m_y{ y },
    m_z{ z }
{}

norm3_t ONB::xAxis() noexcept
{
    return norm3_t{ 1.0, 0.0, 0.0 };
}

norm3_t ONB::yAxis() noexcept
{
    return norm3_t{ 0.0, 1.0, 0.0 };
}

norm3_t ONB::zAxis() noexcept
{
    return norm3_t{ 0.0, 0.0, 1.0 };
}

vec3_t ONB::toLocal(const vec3_t& world) const noexcept
{
    return vec3_t{
        world.dot(m_x),
        world.dot(m_y),
        world.dot(m_z)
    };
}

vec3_t ONB::toWorld(const vec3_t& local) const noexcept
{
    return m_x * local.x + m_y * local.y + m_z * local.z;
}

ONB ONB::fromZ(const norm3_t& z) noexcept
{
    static const auto alike = 0.9999;
    const auto xx = (std::abs(z.dot(ONB::xAxis())) > alike ? ONB::yAxis() : ONB::xAxis()).cross(z).norm();
    const auto yy = z.cross(xx).norm();
    return ONB(xx, yy, z);
}

norm3_t& ONB::x() noexcept
{
    return m_x;
}

const norm3_t& ONB::x() const noexcept
{
    return m_x;
}

norm3_t& ONB::y() noexcept
{
    return m_y;
}

const norm3_t& ONB::y() const noexcept
{
    return m_y;
}

norm3_t& ONB::z() noexcept
{
    return m_z;
}

const norm3_t& ONB::z() const noexcept
{
    return m_z;
}
