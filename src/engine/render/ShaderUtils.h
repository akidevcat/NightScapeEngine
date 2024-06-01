#ifndef SHADERUTILS_H
#define SHADERUTILS_H

#include <d3d11.h>

#include "ConstBufferData.h"

namespace NSE
{
    class ShaderUtils
    {
    public:
        ShaderUtils() = delete;

        static size_t PropertyToID(LPCSTR name);
    };
}

#endif //SHADERUTILS_H
