#ifndef NSECOMMON_H
#define NSECOMMON_H

#define S_PID(NAME) static size_t PID_ ## NAME = ShaderUtils::PropertyToID(#NAME)

#endif //NSECOMMON_H
