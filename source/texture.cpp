#include "texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include <functional>

Texture::Texture(Texture &&that) noexcept
{
    std::swap(m_data, that.m_data);
    std::swap(m_width, that.m_width);
    std::swap(m_height, that.m_height);
}

Texture::Texture(const std::size_t width, const std::size_t height):
    m_data{ std::make_unique<vec3_t[]>(width * height) },
    m_width{ width },
    m_height{ height }
{}

Texture Texture::load(const std::string_view &filename)
{
    Texture texture;

    auto width = 0;
    auto height = 0;
    auto channelsInFile = 0;

    stbi_set_flip_vertically_on_load(true);
    auto data = std::unique_ptr<std::uint8_t[], std::function<void(std::uint8_t *)>>(
            (std::uint8_t *)stbi_load(filename.data(), &width, &height, &channelsInFile, 3),
            [](std::uint8_t *data) { stbi_image_free(data); }
    );
    if(nullptr == data) {
        throw std::runtime_error(filename.data());
    }

    texture.m_width = width;
    texture.m_height = height;
    texture.m_data = std::make_unique<vec3_t[]>(texture.m_width * texture.m_height);

    for(auto y = 0; y < texture.m_height; ++y) {
        for(auto x = 0; x < texture.m_width; ++x) {
            texture.m_data[y * texture.m_width + x] = vec3_t{
                (double)data[(y * width + x) * 3 + 0] / std::numeric_limits<std::uint8_t>::max(), // r
                (double)data[(y * width + x) * 3 + 1] / std::numeric_limits<std::uint8_t>::max(), // g
                (double)data[(y * width + x) * 3 + 2] / std::numeric_limits<std::uint8_t>::max()  // b
            };
        }
    }

    return texture;
}

vec3_t Texture::sample(const double s, const double t) const noexcept
{
    const auto x = std::clamp((std::size_t)std::floor(s * m_width ), 0ul, m_width  - 1);
    const auto y = std::clamp((std::size_t)std::floor(t * m_height), 0ul, m_height - 1);
    return m_data[y * m_width + x];
}