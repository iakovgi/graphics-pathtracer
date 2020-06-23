#ifndef PATHTRACER_SCENELOADER_H__
#define PATHTRACER_SCENELOADER_H__

#include "scene.h"

#include <string_view>

class SceneLoader
{
public:
    static Scene loadScene(const std::string_view& fileName);

private:
    virtual ~SceneLoader() = 0;
};

#endif //PATHTRACER_SCENELOADER_H__
