#include "SceneServer.h"

SceneServer::SceneServer()
{

}

SceneServer::~SceneServer()
{
    for (auto& it : _scenes)
    {
        delete it.second;
    }
}

bool SceneServer::Initialize()
{
    return true;
}

size_t SceneServer::CreateScene(_Out_ Scene*& scene)
{
    scene = new Scene{};
    _scenes.emplace(scene->GetUID(), scene);
    return scene->GetUID();
}

void SceneServer::UnloadScene(size_t sceneId)
{
    delete _scenes.at(sceneId);
    _scenes.erase(sceneId);
}

void SceneServer::GetAllScenes(std::vector<Scene*>& vec)
{
    for (auto& it : _scenes)
    {
        vec.emplace_back(it.second);
    }
}
