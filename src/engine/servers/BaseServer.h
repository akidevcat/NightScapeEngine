#ifndef BASESERVER_H
#define BASESERVER_H

#include <cassert>
#include <type_traits>

namespace NSE
{

    template<class T>
    class BaseServer
    {
    protected:
        BaseServer();
        ~BaseServer();

    public:
        static T* Get()
        {
            assert(_instance != nullptr);
            return _instance;
        }

    private:
        inline static T* _instance = nullptr;
    };

    template<class T>
    BaseServer<T>::BaseServer()
    {
        static_assert(std::is_base_of_v<BaseServer, T>);
        _instance = static_cast<T*>(this);
    }

    template<class T>
    BaseServer<T>::~BaseServer()
    {
        _instance = nullptr;
    }

}

#endif //BASESERVER_H
