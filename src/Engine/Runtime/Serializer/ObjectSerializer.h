#pragma once

namespace NSE
{
    struct IObjectSerializer : public RefCounted
    {
    public:
        IObjectSerializer() = default;
        ~IObjectSerializer() override = default;

        template <typename T>
        T& As() const { return dynamic_cast<T*>(this); }

        virtual std::string Serialize(const void* object) const = 0;
        virtual bool Deserialize(const std::string& textObject, void* result) const = 0;
    };

    template <typename T>
    class ObjectSerializer : public IObjectSerializer
    {
    public:
        ObjectSerializer()
        {
            _serializedObjectTypeHash = typeid(T).hash_code();
        }
        ~ObjectSerializer() override = default;

        virtual std::string Serialize(const T* object) const { return {}; }
        virtual bool Deserialize(const std::string& textObject, T* result) const { return false; }

        std::string Serialize(const void* object) const final { return Serialize(static_cast<const T*>(object)); }
        bool Deserialize(const std::string& textObject, void* result) const final { return Deserialize(textObject, static_cast<T*>(result)); }

        [[nodiscard]] size_t objectTypeHash() const { return _serializedObjectTypeHash; }

    private:
        size_t _serializedObjectTypeHash = 0;
    };
}
