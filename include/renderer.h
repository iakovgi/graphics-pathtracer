#ifndef PATHTRACER_RENDERER_H__
#define PATHTRACER_RENDERER_H__

#include "ptmath.h"
#include "scene.h"

#include <cstdlib>
#include <thread>
#include <iostream>
#include <iomanip>
#include <future>

class Renderer
{
public:
    struct render_params_t
    {
        bool preview;

        size_t nThreads;
        
        size_t msaa;
        size_t spp;
        
        size_t minDepth;
        size_t maxDepth;
    };
    
    Renderer(const render_params_t& params) noexcept;
    ~Renderer() noexcept = default;
    
    template <typename FBOType>
    void render(const Scene& scene, FBOType& fbo, const std::atomic<bool>& stop) const noexcept;
    
    Renderer(const Renderer& that) noexcept = default;
    Renderer(Renderer&& that) noexcept = default;
    
    Renderer& operator=(const Renderer& that) noexcept = default;
    Renderer& operator=(Renderer&& that) noexcept = default;
    
private:
    vec3_t radiance(Random& rng, const Scene& scene, const ray_t& ray, const int depth = 0) const noexcept;
    ray_t getNextRay(Random& rng, const ray_t& ray, const Object::hit_t& hit) const noexcept;
    
private:
    render_params_t m_params;
};

template <typename FBOType>
void Renderer::render(const Scene& scene, FBOType& fbo, const std::atomic<bool>& stop) const noexcept
{
    scene.camera.setViewPortSize(fbo.width() * m_params.msaa, fbo.height() * m_params.msaa);
    
    for(auto py = 0u; py < fbo.height(); ++py) {
        for(auto px = 0u; px < fbo.width(); ++px) {
            fbo[py * fbo.width() + px] = vec3_t{};
        }
    }
    
    static const auto renderPixel = [&](size_t px, size_t py) {
        Random rng{};
        auto p_color = vec3_t{ 0.0, 0.0, 0.0 };
        for(auto s = 0u; s < m_params.spp && !stop; ++s) {
            auto s_color = vec3_t{ 0.0 ,0.0, 0.0 };
            for(auto spy = 0u; spy < m_params.msaa; ++spy) {
                const auto cam_spy = py * m_params.msaa + spy;
                for(auto spx = 0u; spx < m_params.msaa; ++spx) {
                    const auto cam_spx = px * m_params.msaa + spx;
                    const auto ray = scene.camera.randomRay(rng, cam_spx, cam_spy);
                    s_color = s_color + radiance(rng, scene, ray);
                }
            }
            s_color = s_color / (double)(m_params.msaa * m_params.msaa);
            p_color = p_color + s_color;
        }
        p_color = p_color / (double)(m_params.spp);
        auto latch = std::lock_guard<std::mutex>(fbo.mutex);
        fbo[py * fbo.width() + px] = p_color;
    };
    
    static const auto renderLine = [&](const size_t py) {
        for(auto px = 0u; px < fbo.width(); ++px) {
            renderPixel(px, py);
        }
    };
    
    auto atomicLine = std::atomic<size_t>{};
    
    static const auto threadRoutine = [&]() {
        while(true) {
            const auto line = atomicLine.fetch_add(1);
            if(line >= fbo.height()) {
                return;
            }
            renderLine(line);
        }
    };
    
    auto threads = std::vector<std::thread>(m_params.nThreads);
    for(auto& t: threads) {
        t = std::thread(threadRoutine);
    }
    
    for(auto& t: threads) {
        t.join();
    }
}

#endif // PATHTRACER_RENDERER_H__
