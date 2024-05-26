#include "ConstBufferData.h"

#include <iostream>

ConstBufferData::ConstBufferData(ID3D11Device* device, UINT id, size_t uid, size_t size, void* customDataPtr)
{
    this->_isCustomDataPtr = true;
    this->id = id;
    this->uid = uid;
    this->size = size;
    this->description = {};
    if (size > 0)
    {
        // this->vPtr = malloc(size);
        // memset(this->vPtr, 0, size);
        this->vPtr = customDataPtr;

        D3D11_BUFFER_DESC bufferDesc;

        bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
        bufferDesc.ByteWidth = size;
        bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        bufferDesc.MiscFlags = 0;
        bufferDesc.StructureByteStride = 0;

        HRESULT result = device->CreateBuffer(&bufferDesc, nullptr, &this->bPtr); // ToDo initial data
        if (FAILED(result))
        {
            std::cerr << "Failed to create constant buffer: " << result << std::endl;
            return;
        }
    }
}

ConstBufferData::ConstBufferData(ID3D11Device* device, UINT id, size_t uid, D3D11_SHADER_BUFFER_DESC desc, size_t size)
{
    this->id = id;
    this->uid = uid;
    this->size = size;
    this->description = desc;
    if (size > 0)
    {
        this->vPtr = malloc(size);
        memset(this->vPtr, 0, size);

        D3D11_BUFFER_DESC bufferDesc;

        bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
        bufferDesc.ByteWidth = size;
        bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        bufferDesc.MiscFlags = 0;
        bufferDesc.StructureByteStride = 0;

        HRESULT result = device->CreateBuffer(&bufferDesc, nullptr, &this->bPtr); // ToDo initial data
        if (FAILED(result))
        {
            std::cerr << "Failed to create constant buffer: " << result << std::endl;
            return;
        }
    }
}

ConstBufferData::~ConstBufferData()
{
    if (vPtr && !_isCustomDataPtr)
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