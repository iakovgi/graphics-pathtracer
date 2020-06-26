#include "sampler.h"

norm3_t Sampler::coneSample(Random &rng, const OrthoNormalBasis &onb, real_t aperture) noexcept
{
    const auto phi = 2 * M_PI * rng.getUniform();
    const auto theta = (aperture / 2.0) * rng.getUniform();

    const auto local = vec3_t{
        std::sin(theta) * std::cos(phi),
        std::sin(theta) * std::sin(phi),
        std::cos(theta)
    };

    return onb.toWorld(local);
}
