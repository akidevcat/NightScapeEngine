#pragma once

namespace NSE
{
    class IEditorInstance
    {
    public:
        virtual ~IEditorInstance() = default;

        virtual void OnInitialize(IAppInstance* app) {}
        virtual void OnInitializeAfterApp(IAppInstance* app) {}

        virtual void OnDispose() {}
    };
}