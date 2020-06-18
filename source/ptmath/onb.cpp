#include "ptmath.h"

Onb::Onb() noexcept:
    m_x{ Onb::xAxis() },
    m_y{ Onb::yAxis() },
    m_z{ Onb::zAxis() }
{}

Onb::Onb(const norm3_t& x, const norm3_t& y, const norm3_t& z) noexcept:
    m_x{ x },
    m_y{ y },
    m_z{ z }
{}

norm3_t Onb::xAxis() noexcept
{
    return norm3_t{ 1.0, 0.0, 0.0 };
}

norm3_t Onb::yAxis() noexcept
{
    return norm3_t{ 0.0, 1.0, 0.0 };
}

norm3_t Onb::zAxis() noexcept
{
    return norm3_t{ 0.0, 0.0, 1.0 };
}

vec3_t Onb::toLocal(const vec3_t& world) const noexcept
{
    return vec3_t{
        world.dot(m_x),
        world.dot(m_y),
        world.dot(m_z)
    };
}

vec3_t Onb::toWorld(const vec3_t& local) const noexcept
{
    return m_x * local.x + m_y * local.y + m_z * local.z;
}

Onb Onb::fromZ(const norm3_t& z) noexcept
{
    static const auto alike = 0.9999;
    const auto xx = (std::abs(z.dot(Onb::xAxis())) > alike ? Onb::yAxis() : Onb::xAxis()).cross(z).norm();
    const auto yy = z.cross(xx).norm();
    return Onb(xx, yy, z);
}

norm3_t& Onb::x() noexcept
{
    return m_x;
}

const norm3_t& Onb::x() const noexcept
{
    return m_x;
}

norm3_t& Onb::y() noexcept
{
    return m_y;
}

const norm3_t& Onb::y() const noexcept
{
    return m_y;
}

norm3_t& Onb::z() noexcept
{
    return m_z;
}

const norm3_t& Onb::z() const noexcept
{
    return m_z;
}
