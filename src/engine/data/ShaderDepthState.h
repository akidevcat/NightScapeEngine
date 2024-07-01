#ifndef SHADERDEPTHSTATE_H
#define SHADERDEPTHSTATE_H
#include <cstdint>

namespace NSE
{
    enum class ShaderDepthState : uint8_t
    {
        Disabled = 0b00000000,
        OnlyRead = 0b00000001,
        ReadWrite = 0b00000011
    };
}

#endif //SHADERDEPTHSTATE_H
