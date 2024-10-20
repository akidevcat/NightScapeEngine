#pragma once

#include "../Core/Ref.h"

namespace NSE
{
    class Asset : public RefCounted
    {
    public:
        Asset() = default;
        ~Asset() override = default;

        void LoadFromFile(const std::string& assetPath)
        {
            std::hash<std::string> hasher;
            _assetPath = assetPath;
            _assetUid = hasher(assetPath);
            OnLoadFromFile(assetPath);
            _assetLoaded = true;
        }

        void ReloadFromFile()
        {
            LoadFromFile(_assetPath);
        }

        [[nodiscard]] size_t GetAssetUID() const { return _assetUid; }
        [[nodiscard]] const std::string& GetAssetPath() const { return _assetPath; }

    protected:
        virtual void OnLoadFromFile(const std::string& assetPath) {}

    private:
        bool _assetLoaded = false;
        size_t _assetUid = 0;
        std::string _assetPath{};
    };
}
