#pragma once

#include <unordered_map>

#include "Scene.h"
#include "../../../../pch.h"

#include "../main/EngineServer.h"

namespace NSE
{
    class Engine;

    class SceneServer : public EngineServer
    {
    public:
        SceneServer();
        ~SceneServer() override;

        bool OnInitialize() override;
        void OnDispose() override;

        Ref<Scene> CreateScene();
        Ref<Scene> GetScene(size_t sceneID);
        void UnloadScene(size_t sceneID);

        std::vector<Ref<Scene>>::iterator begin() { return _cachedScenesList.begin(); }
        std::vector<Ref<Scene>>::iterator end() { return _cachedScenesList.end(); }
        [[nodiscard]] std::vector<Ref<Scene>>::const_iterator begin() const { return _cachedScenesList.begin(); }
        [[nodiscard]] std::vector<Ref<Scene>>::const_iterator end() const { return _cachedScenesList.end(); }
        std::vector<Ref<Scene>>::reverse_iterator rbegin() { return _cachedScenesList.rbegin(); }
        std::vector<Ref<Scene>>::reverse_iterator rend() { return _cachedScenesList.rend(); }
        [[nodiscard]] std::vector<Ref<Scene>>::const_reverse_iterator rbegin() const { return _cachedScenesList.rbegin(); }
        [[nodiscard]] std::vector<Ref<Scene>>::const_reverse_iterator rend() const { return _cachedScenesList.rend(); }

    private:
        void RefreshCachedScenesList();

    private:
        std::unordered_map<size_t, std::shared_ptr<Scene>> _scenes
            = std::unordered_map<size_t, std::shared_ptr<Scene>>();
        std::vector<Ref<Scene>> _cachedScenesList;

        friend NSE::Engine;
    };
}
