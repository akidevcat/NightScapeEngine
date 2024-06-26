#ifndef OBJECT_H
#define OBJECT_H

#define NSE_Object obj_ptr<NSE::Object>

namespace NSE
{
    class Object
    {
    public:
        Object();
        virtual ~Object() = 0;

        [[nodiscard]] size_t GetUID() const { return _uid; }

    protected:
        virtual void OnCreated() {}
        virtual void OnDestroy() {}

    private:
        static size_t _uidCount;

        size_t _uid;
        bool _isDisposed = false;

        friend class ObjectServer;
    };
}

#endif //OBJECT_H
