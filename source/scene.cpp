#include "scene.h"

#include <algorithm>

std::optional<Object::hit_t> Scene::hit(const ray_t& ray) const noexcept
{
    auto nearest = std::numeric_limits<double>::max();
    std::optional<Object::hit_t> hit;
    for(auto& object: m_objects) {
        auto tmp = object->hit(ray);
        if(tmp && tmp->intersection.distance < nearest) {
            hit = tmp;
            nearest = hit->intersection.distance;
        }
    }
    return hit;
}

Scene::Scene(Scene&& that) noexcept
{
    ambient = that.ambient;
    std::swap(m_objects, that.m_objects);
}

ray_t Scene::randomRay(Random& rng, size_t px, size_t py) const noexcept
{
    return camera.randomRay(rng, px, py);
}
