#ifndef PATHTRACER_TEXTURE_H__
#define PATHTRACER_TEXTURE_H__

#include "ptmath.h"

#include <cstdint>
#include <memory>
#include <string_view>

class Texture
{
public:
    Texture(Texture&& that) noexcept;
    Texture(std::size_t width, std::size_t height);

    static Texture load(const std::string_view& filename);

    vec3_t sample(double s, double t) const noexcept;

    ~Texture() noexcept = default;

    Texture(const Texture& that) noexcept = delete;
    Texture& operator=(const Texture& that) noexcept = delete;
    Texture& operator=(Texture&& that) noexcept = delete;

private:
    Texture() noexcept = default;

private:
    std::size_t m_width;
    std::size_t m_height;

    std::unique_ptr<vec3_t[]> m_data;
};

#endif //PATHTRACER_TEXTURE_H__
