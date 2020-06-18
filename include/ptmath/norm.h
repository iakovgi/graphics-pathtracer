#ifndef PT_NORM_H__
#define PT_NORM_H__

struct vec3_t;

struct norm3_t
{
    union {
        struct {
            double x, y, z;
        };
        double data[3];
    };
    
    norm3_t(const norm3_t& that) noexcept = default;
    norm3_t(norm3_t&& that) noexcept = default;

    norm3_t& operator=(const norm3_t& that) noexcept = default;
    norm3_t& operator=(norm3_t&& that) noexcept = default;

    ~norm3_t() noexcept = default;
    
    norm3_t(const vec3_t& that) noexcept;
    operator vec3_t() const noexcept;
    
    double& operator[](const int idx) noexcept;
    const double& operator[](const int idx) const noexcept;

    double len() const noexcept;
    double lenSquared() const noexcept;
    
    vec3_t operator*(const vec3_t& lhs) const noexcept;
    vec3_t operator/(const vec3_t& lhs) const noexcept;
    vec3_t operator+(const vec3_t& lhs) const noexcept;
    vec3_t operator-(const vec3_t& lhs) const noexcept;

    double dot(const vec3_t& lhs) const noexcept;
    vec3_t cross(const vec3_t& lhs) const noexcept;

    vec3_t operator*(const norm3_t& lhs) const noexcept;
    vec3_t operator/(const norm3_t& lhs) const noexcept;
    vec3_t operator+(const norm3_t& lhs) const noexcept;
    vec3_t operator-(const norm3_t& lhs) const noexcept;

    double dot(const norm3_t& lhs) const noexcept;
    vec3_t cross(const norm3_t& lhs) const noexcept;
    
    vec3_t operator*(const double lhs) const noexcept;
    vec3_t operator/(const double lhs) const noexcept;
    
private:
    norm3_t(const double x, const double y, const double z) noexcept;
    friend class ONB;
};

vec3_t operator*(const double rhs, const norm3_t& lhs) noexcept;

#endif // PT_NORM_H__
