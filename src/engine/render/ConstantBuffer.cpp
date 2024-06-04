#include "ConstantBuffer.h"
#include <cassert>

#include "../servers/RenderServer.h"

NSE::ConstantBuffer::ConstantBuffer(size_t nameID) : ConstantBuffer(nameID, 0) { }

NSE::ConstantBuffer::ConstantBuffer(size_t nameID, size_t size)
{
    _nameID = nameID;
    _size = size;

    if (size % 16 != 0)
    {
        _size += 16 - size % 16;
    }

    if (size > 0)
    {
        InitBuffer();
    }
}

void NSE::ConstantBuffer::Resize(size_t size)
{
    if (_size == size || size <= 0)
    {
        return;
    }

    _size = size;

    Release();
    InitBuffer();
    InitBufferData();
}

void NSE::ConstantBuffer::Reflect(ID3D11ShaderReflection *reflection, size_t bufferID, bool autoResize)
{
    _description = new ConstantBufferDescription{reflection, bufferID};
    _isReflected = true;

    if (autoResize)
    {
        Resize(_description->GetDescription().Size);
    }
}

void NSE::ConstantBuffer::EnableBufferData()
{
    _isBufferDataEnabled = true;

    if (_size > 0)
        InitBufferData();
}

void NSE::ConstantBuffer::DisableBufferData()
{
    _isBufferDataEnabled = false;

    delete _data;
    _data = nullptr;
}

void NSE::ConstantBuffer::UploadData(ConstantBufferData *data) const
{
    if (!data->IsDirty())
    {
        return;
    }

    auto deviceContext = RenderServer::Get()->GetDeviceContext();

    D3D11_MAPPED_SUBRESOURCE mappedResource = {};

    assert(("Failed to map constant buffer",
        SUCCEEDED(deviceContext->Map(_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource))));

    memcpy(mappedResource.pData, data->GetDataPointer(), _size);

    deviceContext->Unmap(_buffer, 0);

    data->ResetDirty();
}

void NSE::ConstantBuffer::UploadData() const
{
    assert(_isBufferDataEnabled);
    assert(_data);

    UploadData(_data);
}

void NSE::ConstantBuffer::Release()
{
    if(_buffer)
    {
        _buffer->Release();
        _buffer = nullptr;
    }

    delete _data;
}

void NSE::ConstantBuffer::InitBuffer()
{
    assert(!_buffer);
    assert(_size > 0);

    D3D11_BUFFER_DESC bufferDesc;

    bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    bufferDesc.ByteWidth = _size;
    bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    bufferDesc.MiscFlags = 0;
    bufferDesc.StructureByteStride = 0;

    assert(("Failed to create a buffer", SUCCEEDED(RenderServer::Get()->GetDevice()->CreateBuffer(&bufferDesc, nullptr, &_buffer)))); // ToDo initial data
}

void NSE::ConstantBuffer::InitBufferData()
{
    assert(!_data);
    assert(_size > 0);

    _data = new ConstantBufferData{_size};
}
