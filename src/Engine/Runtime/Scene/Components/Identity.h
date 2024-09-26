#pragma once
#include <string>

namespace NSE::Components
{
    struct Identity
    {
        size_t uid = 0;
        size_t sceneUid = 0;
        std::string name = "Unknown";
    };
}
