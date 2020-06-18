#ifndef PATHTRACER_OBJECT_H__
#define PATHTRACER_OBJECT_H__

#include "ptmath.h"
#include "material.h"

#include <optional>

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

class SphereObject final: public Object
{
public:
    virtual std::optional<hit_t> hit(const ray_t& ray) const noexcept override;
    
    SphereObject(const double radius, const vec3_t& position, const material_t& material) noexcept;
    
    virtual ~SphereObject() noexcept override = default;
    
    SphereObject(const SphereObject& that) noexcept = default;
    SphereObject(SphereObject&& that) noexcept = default;
    
    SphereObject& operator=(const SphereObject& that) noexcept = default;
    SphereObject& operator=(SphereObject&& that) noexcept = default;

private:
    Sphere m_primitive;
    material_t m_material;
};

class TriangleObject final: public Object
{
public:
    virtual std::optional<hit_t> hit(const ray_t& ray) const noexcept override;
    
    TriangleObject(const vec3_t& v0, const vec3_t& v1, const vec3_t& v2, const material_t& material) noexcept;
    
    virtual ~TriangleObject() noexcept override = default;
    
    TriangleObject(const TriangleObject& that) noexcept = default;
    TriangleObject(TriangleObject&& that) noexcept = default;
    
    TriangleObject& operator=(const TriangleObject& that) noexcept = default;
    TriangleObject& operator=(TriangleObject&& that) noexcept = default;

private:
    Triangle m_primitive;
    material_t m_material;
};

#endif // PATHTRACER_OBJECT_H__
