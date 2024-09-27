#pragma once

#include <SDL_video.h>
#include <unordered_map>
#include <vcruntime_typeinfo.h>
#include <vector>

#include "EngineConfiguration.h"
#include "EngineServer.h"
#include "IAppInstance.h"

namespace NSE
{
    class Engine
    {
    public:
        explicit Engine(IAppInstance* app);
        ~Engine() = default;

        void Initialize(const EngineConfiguration& config);
        void UpdateFrame();
        void Shutdown();

        template <typename ST>
        ST* GetServer();

    private:
        template <typename T>
        void RegisterServer(EngineServer<T>* server)
        {
            assert(server);

            if (!server)
                return;

            _servers.emplace_back(server);
            _serversMap.emplace(typeid(*server).hash_code(), server);

            server->OnInitialize();
        }

    private:
        IAppInstance* _app = nullptr;
        EngineConfiguration _config;
        std::vector<IEngineServer*> _servers;
        std::unordered_map<size_t, IEngineServer*> _serversMap;

        bool _isInitialized = false;
        SDL_Window* _window = nullptr;
        SDL_Surface* _surface = nullptr;
    };

    template<typename ST>
    ST* Engine::GetServer()
    {
        const auto result = _serversMap.find(typeid(ST).hash_code());
        assert(result != _serversMap.end());
        return dynamic_cast<ST*>(result->second);
    }
}
