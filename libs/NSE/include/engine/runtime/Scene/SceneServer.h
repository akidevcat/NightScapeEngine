#pragma once

#include <unordered_map>

#include "Scene.h"

#include "../Core/EngineServer.h"
#include "../Memory/FactoryRegistry.h"

namespace NSE
{
    class Engine;

    class SceneServer : public EngineServer<SceneServer>
    {
    public:
        SceneServer();
        ~SceneServer() override;

    protected:
        bool OnInitialize() override;
        void OnDispose() override;

        void BeginFrameUpdate();
        void UpdateFrame();
        void EndFrameUpdate();

    public:
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

        Ref<FactoryRegistry> GetFactoryRegistry() const { return _factoryRegistry; }

    private:
        void RefreshCachedScenesList();

    private:
        std::unordered_map<size_t, SRef<Scene>> _scenes
            = std::unordered_map<size_t, SRef<Scene>>();
        std::vector<Ref<Scene>> _cachedScenesList;

        SRef<FactoryRegistry> _factoryRegistry;

        friend NSE::Engine;
    };

    constexpr EngineServerAccessor<SceneServer> sScene;
}
