#pragma once

#define S_PID(NAME) static size_t PID_ ## NAME = ShaderUtils::PropertyToID(#NAME)

