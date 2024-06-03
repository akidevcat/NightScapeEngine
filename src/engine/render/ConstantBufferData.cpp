#include "ConstantBufferData.h"

#include <cassert>
#include <cstdlib>
#include <cstring>

ConstantBufferData::ConstantBufferData(size_t size)
{
    _size = size;
    _dataPtr = malloc(size);
    memset(_dataPtr, 0, size);
}

void ConstantBufferData::Set(size_t valueOffset, void *value, size_t valueSize)
{
    assert(("Write outside of data memory", valueOffset + valueSize <= _size));

    memcpy(static_cast<char*>(_dataPtr) + valueOffset, value, valueSize);

    _isDirty = true;
}

void ConstantBufferData::Set(void *structPtr, void *memberPtr, void *value, size_t valueSize)
{
    Set(static_cast<char*>(memberPtr) - static_cast<char*>(structPtr), value, valueSize);

    _isDirty = true;
}
