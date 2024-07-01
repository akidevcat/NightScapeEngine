#include "GraphicsBuffer.h"

#include "../servers/RenderServer.h"
#include "../servers/TimeServer.h"

NSE::GraphicsBuffer::GraphicsBuffer(const Target target, const size_t size, const bool keepDataOnCPU)
    : _target(target), _keepDataOnCPU(keepDataOnCPU), _size(size)
{
    if (size % 16 != 0)
    {
        _size += 16 - size % 16;
    }

    if (size > 0)
    {
        InitializeBuffer();
        if (keepDataOnCPU)
        {
            InitializeData();
        }
    }
}

NSE::GraphicsBuffer::GraphicsBuffer(Target target, size_t stride, size_t count, bool keepDataOnCPU)
    : _target(target), _keepDataOnCPU(keepDataOnCPU), _stride(stride)
{
    _size = stride * count;

    if (_size % 16 != 0)
    {
        _size += 16 - _size % 16;
    }

    if (_size > 0)
    {
        InitializeBuffer();
        if (keepDataOnCPU)
        {
            InitializeData();
        }
    }
}

NSE::GraphicsBuffer::~GraphicsBuffer()
{
    Release();
}

void NSE::GraphicsBuffer::Set(void const *value, size_t valueSize, size_t offset)
{
    assert(("Write outside of data memory", offset + valueSize <= _size));

    if (_keepDataOnCPU)
    {
        memcpy(static_cast<char*>(_data) + offset, value, valueSize);
        _isDirty = true;
    }
    else
    {
        Upload(value, valueSize, offset);
    }
}

void NSE::GraphicsBuffer::Resize(size_t newSize)
{
    if (_size == newSize || newSize <= 0)
    {
        return;
    }

    Release();

    _size = newSize;

    if (newSize % 16 != 0)
    {
        _size += 16 - newSize % 16;
    }

    InitializeBuffer();
    if (_keepDataOnCPU)
    {
        InitializeData();
    }
}

void NSE::GraphicsBuffer::Resize(size_t newCount, size_t newStride)
{
    _stride = newStride;

    Resize(newCount * newStride);
}

void NSE::GraphicsBuffer::Upload()
{
    assert(_keepDataOnCPU);

    if (!_isDirty)
    {
        return;
    }

    Upload(_data, _size, 0);

    _isDirty = false;
}

void NSE::GraphicsBuffer::Upload(void const* value, size_t valueSize, size_t offset) const
{
    const auto deviceContext = RenderServer::Get()->GetDeviceContext();

    D3D11_MAPPED_SUBRESOURCE mappedResource = {};

    const auto result = deviceContext->Map(_d3dBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);

    assert(("Failed to map constant buffer", SUCCEEDED(result)));

    memcpy(static_cast<char*>(mappedResource.pData) + offset, value, valueSize);
    // void* test = malloc(valueSize);
    // memset(test, 0, valueSize);
    //
    // if (valueSize == 16)
    // {
    //     memcpy(static_cast<char*>(mappedResource.pData) + offset, test, valueSize);
    // }
    // else
    // {
    //     memcpy(static_cast<char*>(mappedResource.pData) + offset, value, valueSize);
    // }


    // memset(static_cast<char*>(mappedResource.pData) + offset, 0, valueSize);

    deviceContext->Unmap(_d3dBuffer, 0);
}

void NSE::GraphicsBuffer::UploadForce()
{
    _isDirty = true;
    Upload();
}

void NSE::GraphicsBuffer::Release()
{
    if (_d3dBuffer)
    {
        _d3dBuffer->Release();
        _d3dBuffer = nullptr;
    }

    free(_data);
    _data = nullptr;
}

void NSE::GraphicsBuffer::ReflectAsConstantBuffer(ID3D11ShaderReflection *shaderReflection, size_t bufferID, bool autoResize)
{
    assert(_target == Target::Constant);

    auto bufferReflection = shaderReflection->GetConstantBufferByIndex(bufferID);
    ID3D11ShaderReflectionVariable* varReflection = nullptr;

    auto result = bufferReflection->GetDesc(&_d3dDescription);

    assert(("Failed to get constant buffer description", SUCCEEDED(result)));

    D3D11_SHADER_VARIABLE_DESC varDescription = {};

    for (UINT i = 0; i < _d3dDescription.Variables; i++)
    {
        varReflection = bufferReflection->GetVariableByIndex(i);

        result = varReflection->GetDesc(&varDescription);

        assert(("Failed to reflect constant buffer variable", SUCCEEDED(result)));

        _d3dVariableDescriptions.emplace(ShaderUtils::PropertyToID(varDescription.Name), varDescription);
    }

    _isReflected = true;

    if (autoResize)
    {
        Resize(_d3dDescription.Size);
    }
}

bool NSE::GraphicsBuffer::GetD3DVariableDescription(size_t nameID, D3D11_SHADER_VARIABLE_DESC &desc) const
{
    assert(_isReflected);

    auto result = _d3dVariableDescriptions.find(nameID);
    if (result != _d3dVariableDescriptions.end())
    {
        desc = result->second;
        return true;
    }

    return false;
}

void NSE::GraphicsBuffer::InitializeBuffer()
{
    assert(!_d3dBuffer);
    assert(_size > 0);

    const auto render = RenderServer::Get();
    auto flCreateResourceView = false;

    D3D11_BUFFER_DESC bufferDesc;

    bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    bufferDesc.ByteWidth = _size;
    bufferDesc.BindFlags = 0;
    bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    bufferDesc.MiscFlags = 0;
    bufferDesc.StructureByteStride = 0;

    switch (_target)
    {
        case Target::Constant:
            bufferDesc.BindFlags |= D3D11_BIND_CONSTANT_BUFFER;
        break;

        case Target::Index:
            bufferDesc.BindFlags |= D3D11_BIND_INDEX_BUFFER;
        break;

        case Target::Vertex:
            bufferDesc.BindFlags |= D3D11_BIND_VERTEX_BUFFER;
        break;

        case Target::Structured:
            assert(_stride > 0);
            bufferDesc.StructureByteStride = _stride;
            bufferDesc.MiscFlags |= D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
            bufferDesc.BindFlags |= D3D11_BIND_SHADER_RESOURCE;
            flCreateResourceView = true;
        break;

    }

    auto result = render->GetDevice()->CreateBuffer(&bufferDesc, nullptr, &_d3dBuffer);

    assert(("Failed to create a d3d buffer", SUCCEEDED(result))); // ToDo initial data

    if (flCreateResourceView)
    {
        D3D11_SHADER_RESOURCE_VIEW_DESC desc;

        desc.Format = DXGI_FORMAT_UNKNOWN;
        desc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
        desc.Buffer.FirstElement = 0;
        desc.Buffer.NumElements = _size / _stride;

        result = render->GetDevice()->CreateShaderResourceView(_d3dBuffer, &desc, &_resourceView);

        assert(SUCCEEDED(result));
    }
}

void NSE::GraphicsBuffer::InitializeData()
{
    assert(_size > 0);
    assert(_keepDataOnCPU);

    _data = malloc(_size);
    memset(_data, 0, _size);
}