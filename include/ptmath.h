#ifndef PATHTRACER_PTMATH_H__
#define PATHTRACER_PTMATH_H__

#include <cmath>

using real_t = double;

#include "vec.h"
#include "norm.h"
#include "onb.h"
#include "random.h"
#include "ray.h"
#include "primitive.h"

using vec3_t = graphics::math::vec3_t<real_t>;
using norm3_t = graphics::math::norm3_t<real_t>;
using OrthoNormalBasis = graphics::math::OrthoNormalBasis<real_t>;
using Random = graphics::math::Random;
using ray_t = graphics::math::ray_t<real_t>;
using Primitive = graphics::math::Primitive<real_t>;
using SpherePrimitive = graphics::math::SpherePrimitive<real_t>;
using TrianglePrimitive = graphics::math::TrianglePrimitive<real_t>;

static constexpr real_t PTMATH_EPSILON = 32 * std::numeric_limits<float>::epsilon();

#endif // PATHTRACER_PTMATH_H__
