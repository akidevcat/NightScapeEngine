#pragma once

#include "../Core/EngineServer.h"
#include "../Core/EngineConfiguration.h"
#include "../Serializer/SerializerServer.h"

namespace NSE
{
    class AssetsServer : public EngineServer<AssetsServer>
    {
    public:
        AssetsServer(EngineConfiguration cfg);
        ~AssetsServer() override;

        bool OnInitialize() override;
        void OnDispose() override;

        template <typename T>
        SRef<T> Load(const std::string& path)
        {

        }

        template <typename T>
        SRef<T> LoadSerializable(const std::string& path)
        {
            static_assert(std::is_base_of_v<RefCounted, T>, "An attempt to load a non-ref class");
            std::string fileText;

            auto file = SDL_RWFromFile((_applicationBasePath + "/" + path).c_str(), "r");
            assert(("Unable to read from the specified file", file != nullptr));
            if (file == nullptr)
            {
                return nullptr;
            }

            auto fileSize = SDL_RWsize(file);
            fileText.resize(fileSize);
            SDL_RWread(file, &fileText[0], 1, fileSize);
            SDL_RWclose(file);

            auto oResult = SRef{new T{}};
            auto dResult = sSerializer->Deserialize(fileText, oResult.Raw());
            assert(("Unable to deserialize the specified file", dResult));
            if (!dResult)
            {
                return nullptr;
            }

            return oResult;
        }

        template <typename T>
        bool LoadSerializable(const std::string& path, T* result)
        {
            static_assert(std::is_base_of_v<RefCounted, T>, "An attempt to load a non-ref class");
            std::string fileText;

            auto file = SDL_RWFromFile((_applicationBasePath + "/" + path).c_str(), "r");
            assert(("Unable to read from the specified file", file != nullptr));
            if (file == nullptr)
            {
                return false;
            }

            auto fileSize = SDL_RWsize(file);
            fileText.resize(fileSize);
            SDL_RWread(file, &fileText[0], 1, fileSize);
            SDL_RWclose(file);

            auto dResult = sSerializer->Deserialize(fileText, result);
            assert(("Unable to deserialize the specified file", dResult));
            if (!dResult)
            {
                return false;
            }

            return true;
        }

        template <typename T>
        bool SaveSerializable(const std::string& path, const T* object)
        {
            static_assert(std::is_base_of_v<RefCounted, T>, "An attempt to save a non-ref class");
            std::string fileText;

            auto file = SDL_RWFromFile((_applicationBasePath + "/" + path).c_str(), "w+");
            assert(("Unable to write to the specified file", file != nullptr));
            if (file == nullptr)
            {
                return false;
            }

            auto sResult = sSerializer->Serialize(object);

            auto fileSize = sResult.size();
            SDL_RWwrite(file, &fileText[0], 1, fileSize);
            SDL_RWclose(file);

            return true;
        }

        [[nodiscard]] const std::string& GetApplicationBasePath() const { return _applicationBasePath; }

    private:
        EngineConfiguration _config;

        std::string _applicationBasePath;
        char* _applicationBasePathPtr = nullptr;
    };

    constexpr EngineServerAccessor<AssetsServer> sAssets;
}
