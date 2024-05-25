#include "ConstBufferData.h"

#include <iostream>

ConstBufferData::ConstBufferData(UINT id, size_t uid, D3D11_SHADER_BUFFER_DESC desc) : ConstBufferData(id, uid, desc, 0)
{

}

ConstBufferData::ConstBufferData(UINT id, size_t uid, D3D11_SHADER_BUFFER_DESC desc, size_t size)
{
    this->id = id;
    this->uid = uid;
    this->size = size;
    this->description = desc;
    if (size > 0)
    {
        this->vPtr = malloc(size);
        memset(this->vPtr, 0, size);
    }
}

ConstBufferData::~ConstBufferData()
{
    if (vPtr)
        free(vPtr);
}

bool ConstBufferData::SetVar(size_t pUid, void* value, size_t valueSize)
{
    auto propertyDescIt = vDesc.find(pUid);
    if (propertyDescIt == vDesc.end())
    {
        // ToDo
        std::cerr << "Attempt to set an non-existing variable with uid " << pUid << std::endl;
        return false;
    }

    auto propertyDesc = propertyDescIt->second;
    auto varOffset = propertyDesc.StartOffset;
    auto varSize = propertyDesc.Size;

    // auto valuePtr = vPtr + varOffset;
    auto valuePtr = static_cast<char*>(vPtr) + varOffset; // ToDo

    if (varOffset + varSize > description.Size)
    {
        // ToDo
        std::cerr << "Attempt to set a variable with a overflow value, variable uid " << pUid << std::endl;
        return false;
    }
    memcpy(valuePtr, value, valueSize);

    _isDirty = true;
    return true;
}