#pragma once

#define S_PID(NAME) static size_t PID_ ## NAME = ShaderUtils::PropertyToID(#NAME)
#define SPID(NAME) static size_t PID ## NAME = ShaderUtils::PropertyToID(#NAME)

