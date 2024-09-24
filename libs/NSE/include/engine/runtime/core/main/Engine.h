#pragma once

#include <SDL_video.h>
#include <unordered_map>
#include <vcruntime_typeinfo.h>
#include <vector>

#include "EngineConfiguration.h"
#include "EngineServer.h"
#include "../game/IGameInstance.h"

namespace NSE
{
    class Engine
    {
    public:
        explicit Engine(IGameInstance* game);
        ~Engine();

        void Initialize(const EngineConfiguration& config);
        void UpdateFrame();
        void Shutdown();

        template <typename ST>
        ST* GetServer();

    private:
        void RegisterServer(EngineServer* server);

    private:
        IGameInstance* _game = nullptr;
        EngineConfiguration _config;
        std::vector<EngineServer*> _servers;
        std::unordered_map<size_t, EngineServer*> _serversMap;

        bool _isInitialized = false;
        SDL_Window* _window = nullptr;
        SDL_Surface* _surface = nullptr;
        SDL_GLContext _glContext = nullptr;

    };

    template<typename ST>
    ST* Engine::GetServer()
    {
        const auto result = _serversMap.find(typeid(ST).hash_code());
        assert(result != _serversMap.end());
        return static_cast<ST*>(result->second);
    }
}
