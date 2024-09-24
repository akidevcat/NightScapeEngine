#include "SceneServer.h"

NSE::SceneServer::SceneServer()
{
}

NSE::SceneServer::~SceneServer()
{
}

bool NSE::SceneServer::OnInitialize()
{
    return true;
}

void NSE::SceneServer::OnDispose()
{

}

NSE::Ref<NSE::Scene> NSE::SceneServer::CreateScene()
{
    auto result = std::make_shared<Scene>();
    _scenes.emplace(result->GetUID(), result);
    _cachedScenesList.emplace_back(result);
    return result;
}

NSE::Ref<NSE::Scene> NSE::SceneServer::GetScene(size_t sceneID)
{
    auto result = _scenes.find(sceneID);
    if (result == _scenes.end())
        return nullptr;
    return result->second;
}

void NSE::SceneServer::UnloadScene(size_t sceneID)
{
    auto result = _scenes.find(sceneID);
    assert(result != _scenes.end());
    if (result == _scenes.end())
        return;

    result->second->OnDispose();

    // This should destroy the Scene object
    _scenes.erase(result);

    RefreshCachedScenesList();
}

void NSE::SceneServer::RefreshCachedScenesList()
{
    _cachedScenesList.clear();
    for (auto it : _scenes)
    {
        _cachedScenesList.emplace_back(it.second);
    }
}
