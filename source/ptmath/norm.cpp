#include "ptmath.h"

norm3_t::norm3_t(const vec3_t& that) noexcept
{
    const auto len = that.len();
    x = that.x / len;
    y = that.y / len;
    z = that.z / len;
}

norm3_t::operator vec3_t() const noexcept
{
    return vec3_t{ x, y, z };
}

double& norm3_t::operator[](const int idx) noexcept
{
    return data[idx];
}

const double& norm3_t::operator[](const int idx) const noexcept
{
    return data[idx];
}

vec3_t norm3_t::operator*(const vec3_t& lhs) const noexcept
{
    return vec3_t{ x * lhs.x, y * lhs.y, z * lhs.z };
}

vec3_t norm3_t::operator/(const vec3_t& lhs) const noexcept
{
    return vec3_t{ x / lhs.x, y / lhs.y, z / lhs.z };
}

vec3_t norm3_t::operator+(const vec3_t& lhs) const noexcept
{
    return vec3_t{ x + lhs.x, y + lhs.y, z + lhs.z };
}

vec3_t norm3_t::operator-(const vec3_t& lhs) const noexcept
{
    return vec3_t{ x - lhs.x, y - lhs.y, z - lhs.z };
}

double norm3_t::dot(const vec3_t& lhs) const noexcept
{
    return x * lhs.x + y * lhs.y + z * lhs.z;
}

vec3_t norm3_t::cross(const vec3_t& lhs) const noexcept
{
    return vec3_t{
        y * lhs.z - z * lhs.y,
        z * lhs.x - x * lhs.z,
        x * lhs.y - y * lhs.x
    };
}

vec3_t norm3_t::operator*(const norm3_t& lhs) const noexcept
{
    return vec3_t{ x * lhs.x, y * lhs.y, z * lhs.z };
}

vec3_t norm3_t::operator/(const norm3_t& lhs) const noexcept
{
    return vec3_t{ x / lhs.x, y / lhs.y, z / lhs.z };
}

vec3_t norm3_t::operator+(const norm3_t& lhs) const noexcept
{
    return vec3_t{ x + lhs.x, y + lhs.y, z + lhs.z };
}

vec3_t norm3_t::operator-(const norm3_t& lhs) const noexcept
{
    return vec3_t{ x - lhs.x, y - lhs.y, z - lhs.z };
}

double norm3_t::dot(const norm3_t& lhs) const noexcept
{
    return x * lhs.x + y * lhs.y + z * lhs.z;
}

norm3_t::norm3_t(const double x, const double y, const double z) noexcept:
    x{ x },
    y{ y },
    z{ z }
{}

vec3_t norm3_t::cross(const norm3_t& lhs) const noexcept
{
    return vec3_t{
        y * lhs.z - z * lhs.y,
        z * lhs.x - x * lhs.z,
        x * lhs.y - y * lhs.x
    };
}

vec3_t norm3_t::operator*(const double lhs) const noexcept
{
    return vec3_t{ x * lhs, y * lhs, z * lhs };
}

vec3_t norm3_t::operator/(const double lhs) const noexcept
{
    return vec3_t{ x / lhs, y / lhs, z / lhs };
}

vec3_t operator*(const double rhs, const norm3_t& lhs) noexcept
{
    return lhs * rhs;
}
