#include "SceneServer.h"

NSE::SceneServer::SceneServer()
{

}

NSE::SceneServer::~SceneServer()
{
    for (auto& it : _scenes)
    {
        delete it.second;
    }
}

bool NSE::SceneServer::Initialize()
{
    return true;
}

size_t NSE::SceneServer::CreateScene(_Out_ Scene*& scene)
{
    scene = new Scene{};
    _scenes.emplace(scene->GetUID(), scene);
    return scene->GetUID();
}

void NSE::SceneServer::UnloadScene(size_t sceneId)
{
    delete _scenes.at(sceneId);
    _scenes.erase(sceneId);
}

void NSE::SceneServer::GetAllScenes(std::vector<Scene*>& vec)
{
    for (auto& it : _scenes)
    {
        vec.emplace_back(it.second);
    }
}

NSE::Scene* NSE::SceneServer::GetScene(size_t sceneId)
{
    auto result = _scenes.find(sceneId);
    if (result == _scenes.end())
    {
        return nullptr;
    }

    return result->second;
}
