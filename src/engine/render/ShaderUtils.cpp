#include "ShaderUtils.h"

#include <functional>
#include <iostream>

#include "ConstBufferData.h"

size_t NSE::ShaderUtils::PropertyToID(LPCSTR name)
{
    return std::hash<std::string>()(name);
}