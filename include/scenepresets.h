#ifndef PT_SCENEPRESETS_H__
#define PT_SCENEPRESETS_H__

#include "scene.h"
#include "object.h"
#include "material.h"

class ScenePresets
{
public:
    static Scene smallpt() noexcept
    {
        auto scene = Scene();
        const auto camDir = vec3_t{ 0, -0.042612, -1 }.norm();
        const auto camPos = vec3_t{ 50, 52, 295.6 } + camDir * 131.0 + vec3_t{ 0.0, 3.0, 0.0 };
        scene.camera = Camera(camPos, camDir);
        
        scene.addObject<SphereObject>(1e5,  vec3_t{  1e5+1, 40.8, 81.6 },
                                      material_t::createDiffuse(vec3_t{ .75, .25, .25 })); // Left
        scene.addObject<SphereObject>(1e5,  vec3_t{ -1e5+99, 40.8, 81.6 },
                                      material_t::createDiffuse(vec3_t{ .25, .25, .75 })); // Right
        scene.addObject<SphereObject>(1e5,  vec3_t{ 50, 40.8, 1e5 },
                                      material_t::createDiffuse(vec3_t{ .75, .75, .75 })); // Back
        scene.addObject<SphereObject>(1e5,  vec3_t{ 50, 40.8, -1e5+170 },
                                      material_t::createDiffuse(vec3_t{ 0., 0., 0. })); // Front
        scene.addObject<SphereObject>(1e5,  vec3_t{ 50, 1e5, 81.6 },
                                      material_t::createDiffuse(vec3_t{ .75, .75, .75 })); // Bottom
        scene.addObject<SphereObject>(1e5,  vec3_t{ 50, -1e5+81.6, 81.6 },
                                      material_t::createDiffuse(vec3_t{ .75, .75, .75 })); // Top
        scene.addObject<SphereObject>(16.5, vec3_t{ 27, 16.5, 47 },
                                      material_t::createSpecular(vec3_t{ 1.0, 1.0, 1.0 }, 0.1)); // Mirror
        scene.addObject<SphereObject>(16.5, vec3_t{ 73, 16.5, 78 },
                                      material_t::createRefractive(vec3_t{ 1.0, 1.0, 1.0 }, 1.5, 0.0)); // Glass
        scene.addObject<SphereObject>(600,  vec3_t{ 50, 681.6-.27, 81.6 },
                                      material_t::createLight(vec3_t{ 12, 12, 12 })); // Light
        return scene;
    }
    
    static Scene wada() noexcept
    {
        double R=60;
        //double R=120;
        double T=30*M_PI/180.;
        double D=R/cos(T);
        
        Scene scene;
        const auto camDir = vec3_t{ 0, -0.042612, -1 }.norm();
        scene.camera = Camera(vec3_t{ 50, 52, 295.6 }, camDir);
        
        scene.addObject<SphereObject>(1e5, vec3_t{ 50, 100, 0 },
                                      material_t::createLight(vec3_t{ 3, 3, 3 })); // sky
        scene.addObject<SphereObject>(1e5, vec3_t{ 50, -1e5-D-R, 0 },
                                      material_t::createDiffuse(vec3_t{ .1, .1, .1 }));           //grnd

        scene.addObject<SphereObject>(R, vec3_t{ 50, 40.8, 62 } + vec3_t{ cos(T), sin(T), 0 } * D,
                                      material_t::createSpecular(vec3_t{ 1, .3, .3 }, 0.0)); //red
        scene.addObject<SphereObject>(R, vec3_t{ 50, 40.8, 62 } + vec3_t{ -cos(T), sin(T), 0 } * D,
                                      material_t::createSpecular(vec3_t{ .3, 1, .3 }, 0.0)); //grn
        scene.addObject<SphereObject>(R, vec3_t{ 50, 40.8, 62 } + vec3_t{ 0, -1, 0 } * D,
                                      material_t::createSpecular(vec3_t{ .3, .3, 1 }, 0.0)); //blue
        scene.addObject<SphereObject>(R, vec3_t{ 50, 40.8, 62 } + vec3_t{ 0, 0, -1 } * D,
                                      material_t::createSpecular(vec3_t{ .53, .53, .53 }, 0.0)); //back
        scene.addObject<SphereObject>(R, vec3_t{ 50, 40.8, 62 } + vec3_t{ 0, 0, 1 } * D,
                                      material_t::createRefractive(vec3_t{ 1, 1, 1 }, 1.5, 0.0)); //front
        return scene;
    }
    
    static Scene cornellBox() noexcept
    {
        auto scene = Scene();
        
        const auto centre = vec3_t{ 73, 16.5, 78 };
        const auto camPos = centre + vec3_t{ 0.0, 0.0, 1.0 } * (3 * 16.5) + vec3_t{ +20.0, 0.0, 0.0 };
        const auto camDir = (vec3_t{ 27, 16.5, 47 } - camPos).norm();
        scene.camera = Camera(camPos, camDir);
        
        const auto addCube = [&](const vec3_t& low, const vec3_t& high, const material_t& material) {
            auto T = [&](unsigned bit) {
                bool x = bit & 4u;
                bool y = bit & 2u;
                bool z = bit & 1u;
                return vec3_t{
                    x ? low.x : high.x,
                    y ? low.y : high.y,
                    z ? low.z : high.z
                };
            };
            // back face
            scene.addObject<TriangleObject>(T(0b000), T(0b100), T(0b110), material);
            scene.addObject<TriangleObject>(T(0b000), T(0b110), T(0b010), material);
            
            scene.addObject<TriangleObject>(T(0b001), T(0b101), T(0b111), material);
            scene.addObject<TriangleObject>(T(0b001), T(0b111), T(0b011), material);
            scene.addObject<TriangleObject>(T(0b000), T(0b100), T(0b101), material);
            scene.addObject<TriangleObject>(T(0b000), T(0b101), T(0b001), material);
            scene.addObject<TriangleObject>(T(0b010), T(0b110), T(0b111), material);
            scene.addObject<TriangleObject>(T(0b010), T(0b111), T(0b011), material);
            scene.addObject<TriangleObject>(T(0b000), T(0b010), T(0b011), material);
            scene.addObject<TriangleObject>(T(0b000), T(0b011), T(0b001), material);
            scene.addObject<TriangleObject>(T(0b100), T(0b110), T(0b111), material);
            scene.addObject<TriangleObject>(T(0b100), T(0b111), T(0b101), material);
        };
        
        scene.camera = Camera(camPos, camDir);
                        
        scene.addObject<SphereObject>(1e5,  vec3_t{  1e5+1, 40.8, 81.6 },
                                      material_t::createDiffuse(vec3_t{ .75, .25, .25 })); // Left
        scene.addObject<SphereObject>(1e5,  vec3_t{ -1e5+99, 40.8, 81.6 },
                                      material_t::createDiffuse(vec3_t{ .25, .25, .75 })); // Right
        scene.addObject<SphereObject>(1e5,  vec3_t{ 50, 40.8, 1e5 },
                                      material_t::createDiffuse(vec3_t{ .75, .75, .75 })); // Back
        scene.addObject<SphereObject>(1e5,  vec3_t{ 50, 40.8, -1e5+170 },
                                      material_t::createDiffuse(vec3_t{ 0., 0., 0. })); // Front
        scene.addObject<SphereObject>(1e5,  vec3_t{ 50, 1e5, 81.6 },
                                      material_t::createDiffuse(vec3_t{ .75, .75, .75 })); // Bottom
        scene.addObject<SphereObject>(1e5,  vec3_t{ 50, -1e5+81.6, 81.6 },
                                      material_t::createDiffuse(vec3_t{ .75, .75, .75 })); // Top
        auto cubeSize = 35.0;
        auto cubeOrigin = vec3_t{ 27, 16.5, 47 };
        auto cubeLow = cubeOrigin + vec3_t{ -cubeSize / 2.0, -cubeSize / 2.0, -cubeSize / 2.0 };
        auto cubeHigh = cubeOrigin + vec3_t{ cubeSize / 2.0, cubeSize / 2.0, cubeSize / 2.0 };
        
        addCube(cubeLow, cubeHigh, material_t::createSpecular(vec3_t{ 0.2, 0.2, 0.9 }, 0.1));
        
        cubeSize = 14;
        cubeOrigin = cubeOrigin - vec3_t{ 0, -0.042612, -1 }.norm() * 35 - vec3_t{ 0.0, cubeSize * 0.8, 0.0 };
        cubeLow = cubeOrigin + vec3_t{ -cubeSize / 2.0, -cubeSize / 2.0, -cubeSize / 2.0 };
        cubeHigh = cubeOrigin + vec3_t{ cubeSize / 2.0, cubeSize / 2.0, cubeSize / 2.0 };
        addCube(cubeLow, cubeHigh, material_t::createDiffuse(vec3_t{ 0.2, 0.8, 0.2 }));
        
        scene.addObject<SphereObject>(16.5, vec3_t{ 73, 16.5, 78 },
                                      material_t::createRefractive(vec3_t{ 0.8, 0.1, 0.1 }, 1.44, 0.0)); // Glass
        scene.addObject<SphereObject>(600,  vec3_t{ 50, 681.6-.27, 81.6 },
                                      material_t::createLight(vec3_t{ 12, 12, 12 })); // Light
        
        return scene;
    }
    
};

#endif // PT_SCENEPRESETS_H__
