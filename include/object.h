#ifndef PATHTRACER_OBJECT_H__
#define PATHTRACER_OBJECT_H__

#include "ptmath.h"
#include "material.h"
#include "texture.h"

#include <optional>
#include <memory>

class Object
{
public:
    struct hit_t
    {
        Primitive::intersection_t intersection;
        material_t material;
        bool inside;
    };
    
    virtual std::optional<hit_t> hit(const ray_t& ray) const noexcept = 0;
    
    virtual ~Object() noexcept = default;
};

class MonoMaterialSphere final: public Object
{
public:
    virtual std::optional<hit_t> hit(const ray_t& ray) const noexcept override;
    
    MonoMaterialSphere(const double radius, const vec3_t& position, const material_t& material) noexcept;
    
    virtual ~MonoMaterialSphere() noexcept override = default;
    
    MonoMaterialSphere(const MonoMaterialSphere& that) noexcept = default;
    MonoMaterialSphere(MonoMaterialSphere&& that) noexcept = default;
    
    MonoMaterialSphere& operator=(const MonoMaterialSphere& that) noexcept = default;
    MonoMaterialSphere& operator=(MonoMaterialSphere&& that) noexcept = default;

private:
    SpherePrimitive m_primitive;
    material_t m_material;
};

class MonoMaterialTriangle final: public Object
{
public:
    virtual std::optional<hit_t> hit(const ray_t& ray) const noexcept override;
    
    MonoMaterialTriangle(const vec3_t& v0, const vec3_t& v1, const vec3_t& v2, const material_t& material) noexcept;
    
    virtual ~MonoMaterialTriangle() noexcept override = default;
    
    MonoMaterialTriangle(const MonoMaterialTriangle& that) noexcept = default;
    MonoMaterialTriangle(MonoMaterialTriangle&& that) noexcept = default;
    
    MonoMaterialTriangle& operator=(const MonoMaterialTriangle& that) noexcept = default;
    MonoMaterialTriangle& operator=(MonoMaterialTriangle&& that) noexcept = default;

private:
    TrianglePrimitive m_primitive;
    material_t m_material;
};

class TextureTriangle final: public Object
{
public:
    struct tex_coord_t
    {
        double s;
        double t;
    };

    virtual std::optional<hit_t> hit(const ray_t& ray) const noexcept override;

    TextureTriangle(const vec3_t& v0, const tex_coord_t& t0,
                    const vec3_t& v1, const tex_coord_t& t1,
                    const vec3_t& v2, const tex_coord_t& t2,
                    const std::shared_ptr<Texture>& texture) noexcept;

    virtual ~TextureTriangle() noexcept override = default;

    TextureTriangle(const TextureTriangle& that) noexcept = default;
    TextureTriangle(TextureTriangle&& that) noexcept = default;

    TextureTriangle& operator=(const TextureTriangle& that) noexcept = default;
    TextureTriangle& operator=(TextureTriangle&& that) noexcept = default;

private:
    TrianglePrimitive m_primitive;

    tex_coord_t m_t0;
    tex_coord_t m_t1;
    tex_coord_t m_t2;

    std::shared_ptr<Texture> m_texture;
};

class Mesh final: public Object
{
private:
    struct face_t;

public:
    Mesh() noexcept = default;
    virtual ~Mesh() noexcept override = default;

    virtual std::optional<hit_t> hit(const ray_t& ray) const noexcept override;

    Mesh(const std::vector<face_t>& faces,
         const std::vector<vec3_t>& positions,
         const std::vector<TextureTriangle::tex_coord_t>& texCoords,
         const material_t& material,
         const std::shared_ptr<Texture>& texture = nullptr) noexcept;

    Mesh(const Mesh& that) noexcept = default;
    Mesh(Mesh&& that) noexcept = delete;
    Mesh& operator=(const Mesh& that) noexcept = delete;
    Mesh& operator=(Mesh&& that) noexcept = delete;

private:
    struct face_t
    {
        std::uint32_t indices[3];
    };

private:
    std::vector<face_t> m_faces;

    std::vector<vec3_t> m_positions;
    std::vector<TextureTriangle::tex_coord_t> m_texCoords;

    material_t m_material;
    std::shared_ptr<Texture> m_texture;
};

#endif // PATHTRACER_OBJECT_H__
