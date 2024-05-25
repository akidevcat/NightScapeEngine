#include "ShaderUtils.h"

#include <functional>
#include <iostream>

#include "ConstBufferData.h"

size_t ShaderUtils::PropertyToID(LPCSTR name)
{
    return std::hash<std::string>()(name);
}

// bool ShaderUtils::SetVar(ConstBufferData* buffer, size_t pUid, void* value, size_t valueSize)
// {
//     auto bufferPtr = buffer->vPtr;
//
//     auto propertyDescIt = buffer->vDesc.find(pUid);
//     if (propertyDescIt == buffer->vDesc.end())
//     {
//         // ToDo
//         std::cerr << "Attempt to set an non-existing variable with uid " << pUid << std::endl;
//         return false;
//     }
//
//     auto propertyDesc = propertyDescIt->second;
//     auto varOffset = propertyDesc.StartOffset;
//     auto varSize = propertyDesc.Size;
//     auto valuePtr = bufferPtr + varOffset;
//
//     if (varOffset + varSize > buffer->description.Size)
//     {
//         // ToDo
//         std::cerr << "Attempt to set a variable with a overflow value, variable uid " << pUid << std::endl;
//         return false;
//     }
//     memcpy(valuePtr, value, valueSize);
//
//     return true;
// }