#pragma once

#include <cassert>

namespace NSE
{
    class Engine;

    class IEngineServer
    {
    protected:
        virtual ~IEngineServer() = default;

        virtual bool OnInitialize() = 0;
        virtual void OnDispose() = 0;

        friend Engine;
    };

    template <typename T>
    struct EngineServerAccessor
    {
        T* operator->() const
        {
            return T::Get();
        }
    };

    template <typename T>
    class EngineServer : public IEngineServer
    {
    protected:
        EngineServer();
        ~EngineServer() override = default;

        bool OnInitialize() override { return true; }
        void OnDispose() override {}

    public:
        static T* Get()
        {
            assert(_instance != nullptr);
            return _instance;
        }

    private:
        inline static T* _instance = nullptr;
        // inline static EngineServer* _instance = nullptr;

        friend Engine;
    };

    template<typename T>
    EngineServer<T>::EngineServer()
    {
        _instance = static_cast<T*>(this);
    }
}
