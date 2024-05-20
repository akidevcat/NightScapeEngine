#ifndef SCENESERVER_H
#define SCENESERVER_H
#include <unordered_map>

#include "../scene/Scene.h"

class SceneServer
{
public:
            SceneServer();
    bool    Initialize();
            ~SceneServer();

    size_t  CreateScene(_Out_ Scene*& scene);
    //      LoadScene ToDo
    void    UnloadScene(size_t sceneId);
    void    GetAllScenes(vector<Scene*>& vec);

private:
    std::unordered_map<size_t, Scene*> _scenes
        = std::unordered_map<size_t, Scene*>();
};

#endif //SCENESERVER_H
