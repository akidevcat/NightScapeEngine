#include "ShaderUtils.h"

#include <functional>
#include <iostream>

size_t NSE::ShaderUtils::PropertyToID(LPCSTR name)
{
    return std::hash<std::string>()(name);
}