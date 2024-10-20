#pragma once

#include "Asset.h"
#include "../Serializer/ObjectSerializer.h"

namespace NSE
{
    class TextAsset : public Asset
    {

    };

    class TextAssetSerializer final : public ObjectSerializer<TextAsset>
    {
    public:
        std::string Serialize(const TextAsset *object) const override;
        bool Deserialize(const std::string &yamlText, TextAsset *result) const override;
    };
}
