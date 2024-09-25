#pragma once
#include <string>

namespace NSE
{
    struct IdentityComponent
    {
        size_t uid = 0;
        size_t sceneUid = 0;
        std::string name = "Unknown";
    };
}
