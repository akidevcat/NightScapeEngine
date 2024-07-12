#ifndef SCENESERVER_H
#define SCENESERVER_H
#include <unordered_map>

#include "BaseServer.h"
#include "../entity/Camera.h"
#include "../scene/Scene.h"

namespace NSE
{
    class SceneServer : public BaseServer<SceneServer>
    {
    public:
        SceneServer();
        bool    Initialize();
        ~SceneServer();

        size_t  CreateScene(_Out_ Scene*& scene);
        void    UnloadScene(size_t sceneId);
        void    GetAllScenes(std::vector<Scene*>& vec);
        Scene*  GetScene(size_t sceneId);

        // void SetMainCamera(const NSE_Camera& camera) { _mainCamera = camera; }
        // [[nodiscard]] Camera* GetMainCamera() const { return _mainCamera; }

    private:
        std::unordered_map<size_t, Scene*> _scenes
            = std::unordered_map<size_t, Scene*>();

        // Camera* _mainCamera = nullptr;
    };
}

#endif //SCENESERVER_H
