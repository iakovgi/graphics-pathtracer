#ifndef PATHTRACER_SCENE_H__
#define PATHTRACER_SCENE_H__

#include "camera.h"
#include "object.h"
#include "ptmath.h"

#include <memory>

class Scene
{
public:
    Camera camera;
    vec3_t ambient = { 0.0, 0.0, 0.0 };

public:
    Scene() noexcept = default;
    ~Scene() noexcept = default;
    
    std::optional<Object::hit_t> hit(const ray_t& ray) const noexcept;
    
    template <typename ObjectType, typename... Args>
    void addObject(Args... args)
    {
        m_objects.emplace_back(std::make_unique<ObjectType>(args...));
    }
    
    ray_t randomRay(Random& rng, size_t px, size_t py) const noexcept;
    
    Scene(const Scene& that) noexcept = delete;
    Scene(Scene&& that) noexcept;
    
    Scene& operator=(const Scene& that) noexcept = delete;
    Scene& operator=(Scene&& that) noexcept = delete;
    
private:
    std::vector<std::unique_ptr<Object>> m_objects;
};

#endif // PATHTRACER_SCENE_H__
