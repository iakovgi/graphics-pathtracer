#include "object.h"

SphereObject::SphereObject(const double radius, const vec3_t& position, const material_t& material) noexcept:
    m_primitive{ Sphere(radius, position) },
    m_material{ material }
{}

std::optional<Object::hit_t> SphereObject::hit(const ray_t& ray) const noexcept
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

TriangleObject::TriangleObject(const vec3_t& v0, const vec3_t& v1, const vec3_t& v2, const material_t& material) noexcept:
    m_primitive{ Triangle(v0, v1, v2) },
    m_material{ material }
{}

std::optional<Object::hit_t> TriangleObject::hit(const ray_t& ray) const noexcept
{
    const auto intersection = m_primitive.intersect(ray);
    if(!intersection) {
        return {};
    } else {
        const auto orientation = m_primitive.orientation();
        const auto inside = ray.direction.dot(orientation) > 0.0;
        return hit_t{
            intersection.value(),
            m_material,
            inside
        };
    }
}
