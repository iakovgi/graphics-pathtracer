#ifndef PT_SPHERE_H__
#define PT_SPHERE_H__

#include "ptmath/primitive.h"

struct vec3_t;

class Sphere final: public Primitive
{
public:
    virtual std::optional<intersection_t> intersect(const ray_t& ray) const noexcept override;
    
    Sphere(const double radius, const vec3_t position) noexcept;
    
    const double& radius() const noexcept;
    const double& radiusSquared() const noexcept;
    const vec3_t& position() const noexcept;
    
    virtual ~Sphere() noexcept override = default;
    
    Sphere(const Sphere& that) noexcept = default;
    Sphere(Sphere&& that) noexcept = default;
    
    Sphere& operator=(const Sphere& that) noexcept = default;
    Sphere& operator=(Sphere&& that) noexcept = default;
    
private:
    double m_radius;
    double m_radiusSquared;
    vec3_t m_position;
};

#endif // PT_SPHERE_H__
