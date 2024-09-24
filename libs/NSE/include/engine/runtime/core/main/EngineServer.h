#pragma once

#include <cassert>

namespace NSE
{
    class Engine;

    class EngineServer
    {
    protected:
        EngineServer();
        virtual ~EngineServer();

        virtual bool OnInitialize() = 0;
        virtual void OnDispose() = 0;

    public:
        // static T* Get()
        // {
        //     assert(_instance != nullptr);
        //     return _instance;
        // }
    public:
        static EngineServer* Get()
        {
            assert(_instance != nullptr);
            return _instance;
        }

    private:
        inline static EngineServer* _instance = nullptr;

        friend Engine;
    };


}
