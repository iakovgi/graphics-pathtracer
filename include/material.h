#ifndef PATHTRACER_MATERIAL_H__
#define PATHTRACER_MATERIAL_H__

#include "ptmath.h"

enum material_type_t
{
    diffuse,
    specular,
    refractive
};

struct material_t
{
    vec3_t diffuse = { 0.0, 0.0, 0.0 };
    vec3_t emission = { 0.0, 0.0, 0.0 };
    
    double ior = 1.0;
    double roughness = 1.0;
    
    material_type_t material_type;
    
    static material_t createDiffuse(const vec3_t& diffuse) noexcept {
        return material_t{
            .diffuse = diffuse,
            .emission = vec3_t{ 0.0, 0.0, 0.0 },
            .ior = std::numeric_limits<double>::max(),
            .roughness = 1.0,
            .material_type = material_type_t::diffuse
        };
    }
    
    static material_t createLight(const vec3_t& emission) noexcept {
        return material_t{
            .diffuse = vec3_t{ 0.0, 0.0, 0.0 },
            .emission = emission,
            .ior = std::numeric_limits<double>::max(),
            .roughness = 1.0,
            .material_type = material_type_t::diffuse
        };
    }
    
    static material_t createSpecular(const vec3_t& diffuse, const double roughness = 0.0) noexcept {
        return material_t{
            .diffuse = diffuse,
            .emission = vec3_t{ 0.0, 0.0, 0.0 },
            .ior = std::numeric_limits<double>::max(),
            .roughness = roughness,
            .material_type = material_type_t::specular
        };
    }
    
    static material_t createRefractive(const vec3_t& diffuse, const double ior = 1.45, const double roughness = 0.0) noexcept {
        return material_t{
            .diffuse = diffuse,
            .emission = vec3_t{ 0.0, 0.0, 0.0 },
            .ior = ior,
            .roughness = roughness,
            .material_type = material_type_t::refractive
        };
    }
};

#endif
