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
    const auto cameraGaze = vec3_t{ -0.0, -0.0, -1.0 };
    const auto cameraPosition = -2.6 * cameraGaze;
    scene.camera = Camera(cameraPosition, cameraGaze);
    return scene;
}

int main(int argc, char *argv[])
{
    auto scene1 = SceneLoader::loadScene("baseBox.obj");

    RenderWindow window(356, 256);

    const auto scene = debugScene();
    std::cout << "Scene constructed\n";
    const auto params = Renderer::render_params_t {
        .preview = true,

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
