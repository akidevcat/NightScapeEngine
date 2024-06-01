#ifndef OBJECT_H
#define OBJECT_H

#include "../obj_ptr.h"
// #include "../servers/ObjectServer.h" // ToDo is it alright?

#define NSE_Object obj_ptr<NSE::Object>

namespace NSE
{
    class Object
    {
    public:
        Object();
        // virtual ~Object() = 0;

        [[nodiscard]] size_t GetUID() const { return _uid; }

        // virtual void OnDestroy() = 0;

        // template <class T, class... ArgTypes, std::enable_if_t<std::is_base_of_v<Object, T>, int> = 0>
            // static obj_ptr<T> Create(ArgTypes&&... args);
        // {
        //     return ObjectServer::Get()->Create<T, ArgTypes>(std::forward<ArgTypes>(args)...);
        // }
    private:
        static size_t _uidCount;

        size_t _uid;
        bool _isDisposed = false;
    };
}

#endif //OBJECT_H
