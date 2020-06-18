#include "ptmath.h"

Random::Random(std::uint32_t seed):
    m_engine{},
    m_distribution{}
{
    if(seed == 0) {
        seed = (std::uint32_t)std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    }
    m_engine.seed(seed);
}

double Random::getUniform() noexcept
{
    return m_distribution(m_engine);
}

norm3_t Random::coneSample(Random& rng, const Onb& onb, const double aperture) noexcept
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
