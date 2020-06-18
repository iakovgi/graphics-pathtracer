#include "renderer.h"
#include "scenepresets.h"

#include "RenderWindow.h"

#include <future>
#include <functional>

int main(int argc, char *argv[])
{
    RenderWindow window(480, 356);
    const auto scene = ScenePresets::cornellBox();
    
    const auto params = Renderer::render_params_t {
        .preview = true,

        .nThreads = std::thread::hardware_concurrency(),
        .msaa = 2,
        .spp = 96,
        
        .minDepth = 8,
        .maxDepth = 128
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
