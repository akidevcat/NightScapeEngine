#pragma once

#include "../obj_ptr.h"

#define NSE_Object obj_ptr<NSE::Object>

namespace NSE
{
    class Object
    {
    public:
        Object();
        virtual ~Object() = 0;

        [[nodiscard]] size_t GetUID() const { return _uid; }
        [[nodiscard]] NSE_Object Self() const { return _selfPointer; }

    protected:
        virtual void OnCreated() {}
        virtual void OnDestroy() {}

    private:
        static size_t _uidCount;

        size_t _uid;
        obj_ptr<Object> _selfPointer = nullptr;
        bool _isDisposed = false;

        friend class ObjectServer;
        friend class Scene;
    };
}