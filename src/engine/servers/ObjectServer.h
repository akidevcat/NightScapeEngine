#ifndef OBJECTSERVER_H
#define OBJECTSERVER_H

#include <memory>
#include <unordered_map>

#include "BaseServer.h"
#include "../obj_ptr.h"
#include "../entity/Object.h"

namespace NSE
{
    class Scene;

    class ObjectServer : public BaseServer<ObjectServer>
    {
    public:
        friend class Scene;

        ObjectServer();
        ~ObjectServer();

        template <class T, class... ArgTypes, std::enable_if_t<std::is_base_of_v<Object, T>, int> = 0>
            obj_ptr<T> Create(ArgTypes&&... args)
        {
            std::shared_ptr<T> ent = std::make_shared<T>(std::forward<ArgTypes>(args)...);
            _objects.emplace(static_cast<std::shared_ptr<Object>>(ent)->GetUID(), ent);
            return ent;
        }

        void Destroy(const NSE_Object& obj);
        void DestroyNow(const NSE_Object& obj);

    private:
        void DestroyAny(const NSE_Object& obj);
        void DestroyAnyNow(const NSE_Object& obj);

    private:
        std::unordered_map<size_t, std::shared_ptr<Object>> _objects = {};
    };

    // template<class T, class ... ArgTypes, std::enable_if_t<std::is_base_of_v<Object, T>, int>>
    //     obj_ptr<T> ObjectServer::Create(ArgTypes &&...args)
    // {
    //     std::shared_ptr<T> ent = make_shared<T, ArgTypes>(std::forward<ArgTypes>(args)...);
    //     _objects.emplace(static_cast<std::shared_ptr<Object>>(ent)->GetUID(), ent);
    //     return ent;
    // }

    template<class T, class ... ArgTypes, std::enable_if_t<std::is_base_of_v<Object, T>, int> = 0>
        obj_ptr<T> CreateObject(ArgTypes &&...args)
    {
        return ObjectServer::Get()->Create<T>(std::forward<ArgTypes>(args)...);
    }

    inline void DestroyObject(const obj_ptr<Object>& obj)
    {
        ObjectServer::Get()->Destroy(obj);
    }

    inline void DestroyObjectNow(const obj_ptr<Object>& obj)
    {
        ObjectServer::Get()->DestroyNow(obj);
    }

    // typedef Create(x) Object::NSE::Create(x);
}

// #define CreateNSEObject(TYPE, ...) NSE::ObjectServer::Get()->Create<TYPE>(__VA_ARGS__)

#endif //OBJECTSERVER_H
