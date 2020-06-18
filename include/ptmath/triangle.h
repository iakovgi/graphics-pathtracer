#ifndef PT_TRIANGLE_H__
#define PT_TRIANGLE_H__

#include "ptmath/primitive.h"

struct vec3_t;
struct norm3_t;

class Triangle final: public Primitive
{
public:
    virtual std::optional<intersection_t> intersect(const ray_t& ray) const noexcept override;
    
    Triangle(const vec3_t& v0, const vec3_t& v1, const vec3_t& v2) noexcept;
    
    const norm3_t& orientation() const noexcept;
    
    virtual ~Triangle() noexcept override = default;
    
    Triangle(const Triangle& that) noexcept = default;
    Triangle(Triangle&& that) noexcept = default;
    
    Triangle& operator=(const Triangle& that) noexcept = default;
    Triangle& operator=(Triangle&& that) noexcept = default;
    
private:
    vec3_t m_v0;
    vec3_t m_v1;
    vec3_t m_v2;
    
    vec3_t m_v01;
    vec3_t m_v02;
    
    norm3_t m_orientation;
};

#endif // PT_TRIANGLE_H__
