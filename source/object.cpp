#include "object.h"

// MonoMaterialSphere --------------------------------------------------------------------------------------------------

MonoMaterialSphere::MonoMaterialSphere(const double radius, const vec3_t& position, const material_t& material) noexcept:
    m_primitive{ SpherePrimitive(radius, position) },
    m_material{ material }
{}

std::optional<Object::hit_t> MonoMaterialSphere::hit(const ray_t& ray) const noexcept
{
    const auto intersection = m_primitive.intersect(ray);
    if(!intersection) {
        return {};
    } else {
        const auto inside = (ray.origin - m_primitive.position()).lenSquared() < m_primitive.radiusSquared();
        return hit_t{
            intersection.value(),
            m_material,
            inside
        };
    }
}

// ---------------------------------------------------------------------------------------------------------------------

// MonoMaterialTriangle ------------------------------------------------------------------------------------------------

MonoMaterialTriangle::MonoMaterialTriangle(const vec3_t& v0, const vec3_t& v1, const vec3_t& v2, const material_t& material) noexcept:
    m_primitive{ TrianglePrimitive(v0, v1, v2) },
    m_material{ material }
{}

std::optional<Object::hit_t> MonoMaterialTriangle::hit(const ray_t& ray) const noexcept
{
    const auto intersection = m_primitive.intersect(ray);
    if(!intersection) {
        return {};
    }
    const auto norm = m_primitive.norm();
    const auto inside = ray.direction.dot(norm) > 0.0;
    return hit_t{
        intersection.value(),
        m_material,
        inside
    };
}

// ---------------------------------------------------------------------------------------------------------------------

// TextureTriangle -----------------------------------------------------------------------------------------------------

std::optional<Object::hit_t> TextureTriangle::hit(const ray_t &ray) const noexcept
{
    const auto intersection = m_primitive.intersect(ray);
    if(!intersection) {
        return {};
    }
    const auto inside = ray.direction.dot(m_primitive.norm()) > 0.0;

    const auto p1 = intersection->position - m_primitive.v0();
    const auto v01 = m_primitive.v1() - m_primitive.v0();
    const auto v02 = m_primitive.v2() - m_primitive.v0();

    const auto s0Squared = v01.cross(v02).lenSquared();
    const auto beta = std::sqrt(p1.cross(v02).lenSquared() / s0Squared);
    const auto gamma = std::sqrt(p1.cross(v01).lenSquared() / s0Squared);
    const auto alpha = 1.0 - beta - gamma;

    const auto tx = tex_coord_t{
        alpha * m_t0.s + beta * m_t1.s + gamma * m_t2.s,
        alpha * m_t0.t + beta * m_t1.t + gamma * m_t2.t
    };

    const auto material = material_t::createDiffuse(m_texture->sample(tx.s, tx.t));

    return hit_t{
        .intersection = intersection.value(),
        .material = material,
        .inside = inside
    };
}

TextureTriangle::TextureTriangle(const vec3_t& v0, const tex_coord_t& t0,
                                 const vec3_t& v1, const tex_coord_t& t1,
                                 const vec3_t& v2, const tex_coord_t& t2,
                                 const std::shared_ptr<Texture>& texture) noexcept:
    m_primitive{ TrianglePrimitive(v0, v1, v2) },
    m_t0{ t0 },
    m_t1{ t1 },
    m_t2{ t2 },
    m_texture{ texture }
{}

// ---------------------------------------------------------------------------------------------------------------------
