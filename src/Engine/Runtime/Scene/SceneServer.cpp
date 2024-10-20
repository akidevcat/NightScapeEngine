#include "SceneServer.h"

#include "../Memory/FactoryRegistry.h"

NSE::SceneServer::SceneServer()
{
    _factoryRegistry = SRef{new FactoryRegistry{}};
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

void NSE::SceneServer::BeginFrameUpdate()
{
    for (auto& scene : _cachedScenesList)
    {
        scene->OnBeginFrameUpdate();
    }
}

void NSE::SceneServer::UpdateFrame()
{
    for (auto& scene : _cachedScenesList)
    {
        scene->OnFrameUpdate();
    }
}

void NSE::SceneServer::EndFrameUpdate()
{
    for (auto& scene : _cachedScenesList)
    {
        scene->OnEndFrameUpdate();
    }
}

NSE::Ref<NSE::Scene> NSE::SceneServer::CreateScene()
{
    // auto result = std::make_shared<Scene>();
    auto result = SRef{new Scene{}};
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

    result->second->OnUnload();

#ifndef NDEBUG
    Ref<Scene> sceneRef = result->second;
#endif

    // This should destroy the Scene object
    _scenes.erase(result);

#ifndef NDEBUG
    assert(sceneRef);
#endif

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
