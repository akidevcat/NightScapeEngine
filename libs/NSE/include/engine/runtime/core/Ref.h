#pragma once

#include <atomic>
#include <mutex>
#include <unordered_set>

namespace NSE
{
    class RefCounted
    {
    public:
        virtual ~RefCounted() = default;

        void IncRef() const { ++_refCount; }
        void DecRef() const { --_refCount; }
        uint32_t RefCount() const { return _refCount.load(); }

    private:
        static void RegRef(void* ptr);
        static void UnregRef(void* ptr);
        static bool ContainsRef(void* ptr);

    private:
        static std::unordered_set<void*> _references;
        static std::mutex _referencesMutex;

        mutable std::atomic<uint32_t> _refCount = 0;

        template<class T>
        friend class Ref;
        template<class T>
        friend class SRef;
    };

    template<typename T>
    class SRef
    {
    public:
        SRef() = default;
        SRef(nullptr_t) {}
        SRef(T* instance) : _instance(instance) { static_assert(std::is_base_of_v<RefCounted, T>, "An attempt to use a non-refcounted class"); IncRef(); }

        SRef(const SRef& other) : _instance(other._instance) { IncRef(); }

        template<typename T2>
        SRef(const SRef<T2>& other)
        {
            _instance = dynamic_cast<T*>(other._instance);
            IncRef();
        }

        template<typename T2>
        SRef(SRef<T2>&& other)
        {
            _instance = dynamic_cast<T*>(other._instance);
            other._instance = nullptr;
        }

        ~SRef() { DecRef(); }

        SRef& operator=(nullptr_t)
        {
            DecRef();
            _instance = nullptr;
            return *this;
        }

        SRef& operator=(const SRef& other)
        {
            if (this == &other)
                return *this;

            other.IncRef();
            DecRef();

            _instance = other._instance;
            return *this;
        }

        template<typename T2>
        SRef& operator=(const SRef<T2>& other)
        {
            other.IncRef();
            DecRef();

            _instance = other._instance;
            return *this;
        }

        template<typename T2>
        SRef& operator=(SRef<T2>& other)
        {
            DecRef();

            _instance = other._instance;
            other._instance = nullptr;
            return *this;
        }

        bool operator==(const SRef& other) const
        {
            return _instance == other._instance;
        }

        bool operator!=(const SRef& other) const
        {
            return !(*this == other);
        }

        bool EqualsObject(const SRef<T>& other)
        {
            if (!_instance && !other._instance)
                return true;

            if (!_instance || !other._instance)
                return false;

            return _instance == *other._instance;
        }

        operator bool() const { return _instance != nullptr; }

        T* operator->() { return _instance; }
        const T* operator->() const { return _instance; }
        T& operator*() { return *_instance; }
        const T& operator*() const { return *_instance; }
        T* Raw() { return _instance; }
        const T* Raw() const { return _instance; }

        template<typename T2>
        SRef<T2> As() const
        {
            return SRef<T2>(*this);
        }

        void Reset(T* instance = nullptr)
        {
            DecRef();
            _instance = instance;
        }

    private:
        void IncRef() const
        {
            if (!_instance)
                return;

            _instance->IncRef();
            RefCounted::RegRef(static_cast<void*>(_instance));
        }

        void DecRef()
        {
            if (!_instance)
                return;

            _instance->DecRef();

            if (_instance->RefCount() <= 0)
            {
                free(_instance);
                RefCounted::UnregRef(static_cast<void*>(_instance));
                _instance = nullptr;
            }
        }

    private:
        mutable T* _instance = nullptr;

        template<class T2>
        friend class SRef;
    };

    template<typename T>
    class Ref
    {
    public:
        Ref() = default;
        Ref(SRef<T> sref) : _instance(sref.Raw()) {}
        Ref(T* instance) : _instance(instance) {}

        template<typename T2>
        Ref(const Ref<T2>& other)
        {
            _instance = dynamic_cast<T*>(other._instance);
        }

        template<typename T2>
        Ref(Ref<T2>&& other)
        {
            _instance = dynamic_cast<T*>(other._instance);
            other._instance = nullptr;
        }

        T* operator->() { assert(("Null reference occured", Alive())); return _instance; }
        const T* operator->() const { assert(("Null reference occured", Alive())); return _instance; }
        T& operator*() { assert(("Null reference occured", Alive())); return *_instance; }
        const T& operator*() const { assert(("Null reference occured", Alive())); return *_instance; }

        bool Alive() const { return _instance ? RefCounted::ContainsRef(_instance) : false; }
        operator bool() const { return Alive(); }

        template<typename T2>
        Ref<T2> As() const { assert(("Null reference occured", Alive())); return Ref<T2>(dynamic_cast<T2*>(_instance)); }

    private:
        T* _instance = nullptr;

        template<class T2>
        friend class Ref;
    };
}
