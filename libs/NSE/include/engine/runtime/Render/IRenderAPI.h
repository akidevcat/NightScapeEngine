#pragma once

namespace NSE
{
    class IRenderAPI
    {
    public:
        virtual ~IRenderAPI() = default;

        virtual bool OnInitialize() { return true; }
        virtual void OnDispose() {}
    };
}