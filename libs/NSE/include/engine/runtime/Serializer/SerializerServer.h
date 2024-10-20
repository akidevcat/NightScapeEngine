#pragma once

#include "ObjectSerializer.h"
#include "../Core/EngineServer.h"
#include "../Core/EngineConfiguration.h"

namespace NSE
{
    class SerializerServer : public EngineServer<SerializerServer>
    {
    public:
        SerializerServer(EngineConfiguration cfg);
        ~SerializerServer() override;

        bool OnInitialize() override;
        void OnDispose() override;

        template <typename T>
        void RegisterSerializer()
        {
            auto serializer = SRef{new T{}};
            size_t objHash = serializer->objectTypeHash();
            _serializers.emplace(objHash, serializer);
        }

        template <typename T>
        Ref<T> GetObjectSerializer()
        {
            return _serializers.find(typeid(T).hash_code());
        }

        template <typename T>
        std::string Serialize(const T* object) const
        {
            auto serializerIt = _serializers.find(typeid(T).hash_code());
            assert(("Unable to find a serializer", serializerIt != _serializers.end()));
            return serializerIt->second->Serialize(object);
        }

        template <typename T>
        bool Deserialize(const std::string& yamlText, T* result) const
        {
            auto serializerIt = _serializers.find(typeid(T).hash_code());
            assert(("Unable to find a serializer", serializerIt != _serializers.end()));
            return serializerIt->second->Deserialize(yamlText, result);
        }

    private:
        EngineConfiguration _config;

        std::unordered_map<size_t, SRef<IObjectSerializer>> _serializers;
    };

    constexpr EngineServerAccessor<SerializerServer> sSerializer;
}