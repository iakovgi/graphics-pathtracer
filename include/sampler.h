#ifndef PATHTRACER_SAMPLER_H__
#define PATHTRACER_SAMPLER_H__

#include "ptmath.h"

class Sampler
{
public:
    static norm3_t coneSample(Random &rng, const OrthoNormalBasis &onb, const real_t aperture) noexcept;

private:
    virtual ~Sampler() = 0;
};

#endif // PATHTRACER_SAMPLER_H__
