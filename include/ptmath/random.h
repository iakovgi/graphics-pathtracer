#ifndef PATHTRACER_RANDOM_H__
#define PATHTRACER_RANDOM_H__

#include <random>
#include <chrono>

struct norm3_t;
class Onb;

class Random
{
public:
    Random(std::uint32_t seed = 0);
    ~Random() = default;

    Random(const Random& that) = delete;
    Random(Random&& that) = delete;
    Random& operator=(const Random& that) = delete;
    Random& operator=(Random& that) = delete;
    
    double getUniform() noexcept;

    static norm3_t coneSample(Random& rng, const Onb& onb, const double aperture) noexcept;
    
private:
    std::default_random_engine m_engine;
    std::uniform_real_distribution<double> m_distribution;
};

#endif // PATHTRACER_RANDOM_H__
