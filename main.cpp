#include "renderer.h"
#include "scenepresets.hpp"

#include "sceneloader.h"
#include "texture.h"
#include "RenderWindow.h"

#include <future>
#include <functional>

Scene debugScene()
{
    Scene scene;
    scene.camera = Camera(vec3_t{ 0.0, 0.0, 0.0 }, vec3_t{ 0.0, 0.0, -1.0 });

    const auto distance = 2.0;

    std::vector<vec3_t> positions{
        vec3_t{ -0.5, -0.5, -distance }, // 0, bottom left
        vec3_t{  0.5, -0.5, -distance }, // 1, bottom right
        vec3_t{ -0.5,  0.5, -distance }, // 2, top left
        vec3_t{  0.5,  0.5, -distance }  // 3, top right
    };

    std::vector<Mesh::face_t> faces{
        Mesh::face_t{ 0, 1, 2 },
        Mesh::face_t{ 2, 1, 3 }
    };

    scene.addObject<Mesh>(faces, positions, std::vector<TextureTriangle::tex_coord_t>{},
                          material_t::createDiffuse(vec3_t{ 0.8, 0.2, 0.2 }));

    return scene;
}

int main(int argc, char *argv[])
{
    RenderWindow window(480, 356);

    const auto scene = SceneLoader::loadScene("resource/allOptions/allOptionsOld.dae");
    //const auto scene = debugScene();
    std::cout << "Scene constructed\n";
    const auto params = Renderer::render_params_t {
        .preview = false,

        .nThreads = std::thread::hardware_concurrency(),
        .msaa = 2,
        .spp = 256,

        .minDepth = 4,
        .maxDepth = 64
    };
    const auto renderer = Renderer(params);

    auto stop = std::atomic<bool>{ false };
    auto renderThread = std::thread([&](){
        renderer.render(scene, window, std::ref(stop));
    });

    while(!window.shouldClose()) {
        std::this_thread::sleep_for(std::chrono::milliseconds(32)); // 30 fps
        window.swapBuffers();
        window.display();
    }

    stop = true;
    renderThread.join();
    return 0;
}
