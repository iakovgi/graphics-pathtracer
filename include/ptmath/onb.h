#ifndef PATHTRACER_ONB_H__
#define PATHTRACER_ONB_H__

struct norm3_t;
struct vec3_t;

class Onb
{
public:
    static norm3_t xAxis() noexcept;
    static norm3_t yAxis() noexcept;
    static norm3_t zAxis() noexcept;
    
    Onb() noexcept;
    Onb(const norm3_t& x, const norm3_t& y, const norm3_t& z) noexcept;
    
    vec3_t toLocal(const vec3_t& world) const noexcept;
    vec3_t toWorld(const vec3_t& local) const noexcept;
    
    static Onb fromZ(const norm3_t& z) noexcept;
    
    norm3_t& x() noexcept;
    const norm3_t& x() const noexcept;
    
    norm3_t& y() noexcept;
    const norm3_t& y() const noexcept;
    
    norm3_t& z() noexcept;
    const norm3_t& z() const noexcept;
    
    Onb(const Onb& that) noexcept = default;
    Onb(Onb&& that) noexcept = default;
    ~Onb() noexcept = default;
    
    Onb& operator=(const Onb& that) noexcept = default;
    Onb& operator=(Onb&& that) noexcept = default;
    
private:
    norm3_t m_x;
    norm3_t m_y;
    norm3_t m_z;
};

#endif // PATHTRACER_ONB_H__