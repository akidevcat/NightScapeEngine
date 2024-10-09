#include "TextAsset.h"

std::string NSE::TextAssetSerializer::Serialize(const TextAsset *object) const
{
    return "";
}

bool NSE::TextAssetSerializer::Deserialize(const std::string &yamlText, TextAsset *result) const
{
    return false;
}
