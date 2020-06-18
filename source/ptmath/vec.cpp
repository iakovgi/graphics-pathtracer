#include "ptmath.h"

vec3_t::vec3_t(double x, double y, double z) noexcept:
    x{ x },
    y{ y },
    z{ z }
{}

double& vec3_t::operator[](const int idx) noexcept
{
    return data[idx];
}

const double& vec3_t::operator[](const int idx) const noexcept
{
    return data[idx];
}

double vec3_t::len() const noexcept
{
    return std::sqrt(dot(*this));
}

double vec3_t::lenSquared() const noexcept
{
    return dot(*this);
}

norm3_t vec3_t::norm() const noexcept
{
    return norm3_t(*this);
}

vec3_t::operator norm3_t() const noexcept
{
    return this->norm();
}

vec3_t vec3_t::operator*(const vec3_t& lhs) const noexcept
{
    return vec3_t{ x * lhs.x, y * lhs.y, z * lhs.z };
}

vec3_t vec3_t::operator/(const vec3_t& lhs) const noexcept
{
    return vec3_t{ x / lhs.x, y / lhs.y, z / lhs.z };
}

vec3_t vec3_t::operator+(const vec3_t& lhs) const noexcept
{
    return vec3_t{ x + lhs.x, y + lhs.y, z + lhs.z };
}

vec3_t vec3_t::operator-(const vec3_t& lhs) const noexcept
{
    return vec3_t{ x - lhs.x, y - lhs.y, z - lhs.z };
}

double vec3_t::dot(const vec3_t& lhs) const noexcept
{
    return x * lhs.x + y * lhs.y + z * lhs.z;
}

vec3_t vec3_t::cross(const vec3_t& lhs) const noexcept
{
    return vec3_t{
        y * lhs.z - z * lhs.y,
        z * lhs.x - x * lhs.z,
        x * lhs.y - y * lhs.x
    };
}

vec3_t vec3_t::operator*(const double lhs) const noexcept
{
    return vec3_t{ x * lhs, y * lhs, z * lhs };
}

vec3_t vec3_t::operator/(const double lhs) const noexcept
{
    return vec3_t{ x / lhs, y / lhs, z / lhs };
}

vec3_t operator*(const double rhs, const vec3_t& lhs) noexcept
{
    return lhs * rhs;
}

vec3_t vec3_t::operator*(const norm3_t& lhs) const noexcept
{
    return (*this) * ((vec3_t)lhs);
}

vec3_t vec3_t::operator/(const norm3_t& lhs) const noexcept
{
    return (*this) / ((vec3_t)lhs);
}

vec3_t vec3_t::operator+(const norm3_t& lhs) const noexcept
{
    return (*this) + ((vec3_t)lhs);
}

vec3_t vec3_t::operator-(const norm3_t& lhs) const noexcept
{
    return (*this) - ((vec3_t)lhs);
}

double vec3_t::dot(const norm3_t& lhs) const noexcept
{
    return (*this).dot((vec3_t)lhs);
}

vec3_t vec3_t::cross(const norm3_t& lhs) const noexcept
{
    return (*this).cross((vec3_t)lhs);
}
