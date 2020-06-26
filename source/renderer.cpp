#include "renderer.h"

#include "ptmath.h"
#include "sampler.h"

Renderer::Renderer(const Renderer::render_params_t& params) noexcept:
    m_params{ params }
{
    if(m_params.preview) {
        m_params.msaa = 1;
        m_params.spp = 1;
    }
}

ray_t Renderer::getNextRay(Random& rng, const ray_t& ray, const Object::hit_t& hit) const noexcept
{
    static const auto getIdealReflect = [](const norm3_t& rayDir, const norm3_t& norm) -> norm3_t {
        return rayDir - norm * 2.0 * norm.dot(rayDir);
    };
    
    static const auto getReflectivity = [](const norm3_t& rayDir, const norm3_t& norm,
                                           const double ior_in, const double ior_out) -> double {
        
        static const auto getR0 = [](const double ior_in, const double ior_out) -> double {
            const auto sqrtR0 = (ior_in - ior_out) / (ior_in + ior_out);
            return sqrtR0 * sqrtR0;
        };
        
        static const auto pow5 = [](const double v) -> double {
            return v * v * v * v * v;
        };
        
        const auto R0 = getR0(ior_in, ior_out);
        const auto cos_i = -1.0 * rayDir.dot(norm);
        
        if(ior_in <= ior_out) {
            return R0 + (1.0 - R0) * pow5(1.0 - cos_i);
        }
        
        const auto iorRatio = ior_in / ior_out;
        const auto sin_t2 = iorRatio * iorRatio * (1.0 - cos_i * cos_i);
        
        const auto TIR = sin_t2 >= 1.0;
        if(TIR) {
            return 1.0;
        }
        
        const auto cos_t = std::sqrt(1.0 - sin_t2);
        return R0 + (1.0 - R0) * pow5(1.0 - cos_t);
    };
    
    static const auto getIdealTransmit = [](const norm3_t& rayDir, const norm3_t& norm,
                                            const double ior_in, const double ior_out) -> norm3_t {
        const auto cos_i = -1.0 * rayDir.dot(norm);
        const auto iorRatio = ior_in / ior_out;
        const auto sin_t2 = iorRatio * iorRatio * (1.0 - cos_i * cos_i);
        return rayDir * iorRatio + norm * (iorRatio * cos_i - std::sqrt(1.0 - sin_t2));
    };
    
    const auto material = hit.material;
    const auto position = hit.intersection.position;
    const auto normal = hit.intersection.normal;
    const auto rayDir = ray.direction;
    
    switch (material.material_type) {
        case material_type_t::diffuse: {
            const auto hitOnb = OrthoNormalBasis::fromZ(normal);
            const auto nextDir = Sampler::coneSample(rng, hitOnb, M_PI);
            return ray_t(position, nextDir);
        }
        case material_type_t::specular: {
            const auto idealReflectDir = getIdealReflect(rayDir, normal);
            const auto hitOnb = OrthoNormalBasis::fromZ(idealReflectDir);
            const auto aperture = material.roughness * M_PI;
            auto nextDir = Sampler::coneSample(rng, hitOnb, aperture);
            while(normal.dot(nextDir) < 0.0) {
                nextDir = Sampler::coneSample(rng, hitOnb, aperture);
            }
            return ray_t(position, nextDir);
        }
        case material_type_t::refractive: {
            auto ior_in = 1.0;
            auto ior_out = material.ior;
            if(hit.inside) {
                std::swap(ior_in, ior_out);
            }
            const auto reflectivity = getReflectivity(rayDir, normal, ior_in, ior_out);
            const auto p = rng.getUniform();
            if(p < reflectivity) {
                const auto idealReflectDir = getIdealReflect(rayDir, normal);
                const auto hitOnb = OrthoNormalBasis::fromZ(idealReflectDir);
                const auto aperture = material.roughness * M_PI;
                const auto nextDir = Sampler::coneSample(rng, hitOnb, aperture);
                return ray_t(position, nextDir);
            } else {
                const auto idealTransmitDir = getIdealTransmit(rayDir, normal, ior_in, ior_out);
                const auto hitOnb = OrthoNormalBasis::fromZ(idealTransmitDir);
                const auto aperture = material.roughness * M_PI;
                const auto nextDir = Sampler::coneSample(rng, hitOnb, aperture);
                return ray_t(position, nextDir);
            }
        }
    }
    return ray_t(vec3_t{}, vec3_t{});
}

vec3_t operator*(const vec3_t &lhs, const vec3_t &rhs) noexcept
{
    return vec3_t{
        lhs.r * rhs.r,
        lhs.g * rhs.g,
        lhs.b * rhs.b
    };
}

vec3_t Renderer::radiance(Random& rng, const Scene& scene, const ray_t& ray, const int depth) const noexcept
{
    const auto hit = scene.hit(ray);
    if(!hit) {
        return scene.ambient;
    }
    
    const auto material = hit->material;
    const auto continueProbability = std::max(material.diffuse.r, std::max(material.diffuse.g, material.diffuse.b));
    
    auto diffuse = material.diffuse;
    if(m_params.preview) {
        return diffuse * std::pow(hit->intersection.normal.dot(ray.direction), 2);
    }

    if(depth > m_params.minDepth) {
        const auto p = rng.getUniform();
        if(p < continueProbability && depth < m_params.maxDepth) {
            diffuse = diffuse / continueProbability;
        } else {
            return hit->material.emission;
        }
    }
    
    const auto nextRay = getNextRay(rng, ray, hit.value());
    return material.emission + diffuse * radiance(rng, scene, nextRay, depth + 1);
}
