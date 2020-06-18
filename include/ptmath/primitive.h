#ifndef PT_PRIMITIVE_H__
#define PT_PRIMITIVE_H__

struct ray_t;
struct vec3_t;
struct norm3_t;

#include <optional>

class Primitive
{
public:
    struct intersection_t
    {
        double distance;
        vec3_t position;
        norm3_t normal;
    };
    
    virtual std::optional<intersection_t> intersect(const ray_t& ray) const noexcept = 0;
    
    virtual ~Primitive() noexcept = default;
};

#endif // PT_PRIMITIVE_H__
