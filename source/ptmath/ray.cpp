#include "ptmath.h"

ray_t::ray_t(const vec3_t& origin, const norm3_t& direction) noexcept:
    origin{ origin },
    direction{ direction }
{}

ray_t ray_t::fromTwoPoints(const vec3_t from, const vec3_t& to) noexcept
{
    return ray_t(from, to - from);
}
