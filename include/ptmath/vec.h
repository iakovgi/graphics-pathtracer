#ifndef PATHTRACER_VEC_H__
#define PATHTRACER_VEC_H__

struct norm3_t;

struct vec3_t
{
    union {
        struct {
            union { double x, r; };
            union { double y, g; };
            union { double z, b; };
        };
        double data[3];
    };

    vec3_t(const vec3_t& that) noexcept = default;
    vec3_t(vec3_t&& that) noexcept = default;

    vec3_t& operator=(const vec3_t& that) noexcept = default;
    vec3_t& operator=(vec3_t&& that) noexcept = default;

    vec3_t(double x = 0.0, double y = 0.0, double z = 0.0) noexcept;
    ~vec3_t() noexcept = default;

    double& operator[](const int idx) noexcept;
    const double& operator[](const int idx) const noexcept;

    double len() const noexcept;
    double lenSquared() const noexcept;
    norm3_t norm() const noexcept;
    explicit operator norm3_t() const noexcept;
    
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
};

vec3_t operator*(const double rhs, const vec3_t& lhs) noexcept;

#endif // PATHTRACER_VEC_H__
