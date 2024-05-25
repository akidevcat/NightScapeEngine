#ifndef SHADERUTILS_H
#define SHADERUTILS_H

#include <d3d11.h>

#include "ConstBufferData.h"

class ShaderUtils
{
public:
    ShaderUtils() = delete;

public:
    static size_t PropertyToID(LPCSTR name);
    // static bool SetVar(ConstBufferData* buffer, size_t pUid, void* value, size_t valueSize);
    // constexpr static size_t PropertyID(LPCSTR name);
};

#endif //SHADERUTILS_H
