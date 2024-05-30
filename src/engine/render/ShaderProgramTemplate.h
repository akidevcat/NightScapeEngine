// ======================================================
// =============== HEADER TEMPLATE ======================
// ======================================================

// Remove on compilation
// #define SHADER_PROGRAM_TEMPLATE_HEADER
// #define SHADER_PROGRAM_TEMPLATE_SOURCE

#ifndef SHADER_PROGRAM_TEMPLATE_CLASS_NAME
#define SHADER_PROGRAM_TEMPLATE_CLASS_NAME UnknownShader
#endif

#if !defined(SHADER_PROGRAM_TEMPLATE_TYPE_VERTEX) && !defined(SHADER_PROGRAM_TEMPLATE_TYPE_PIXEL)
#define SHADER_PROGRAM_TEMPLATE_TYPE_VERTEX
#endif

#ifdef SHADER_PROGRAM_TEMPLATE_HEADER

#include "ConstBufferData.h"
#include "ShaderUtils.h"
#include <d3d11.h>
#include <d3d11shader.h>
#include <unordered_map>

class SHADER_PROGRAM_TEMPLATE_CLASS_NAME
{
public:
    explicit SHADER_PROGRAM_TEMPLATE_CLASS_NAME(const wchar_t* path);
    ~SHADER_PROGRAM_TEMPLATE_CLASS_NAME();

    void Release();
    bool Compile(ID3D11Device* device);
    bool UploadBuffer(ID3D11DeviceContext* deviceContext, ConstBufferData* buffer);
    // bool UploadDrawBuffer(ID3D11DeviceContext* deviceContext);
    // bool UploadGlobalBuffer(ID3D11DeviceContext* deviceContext);

    bool SetGlobalVar(size_t pUid, void* value, size_t valueSize);
    bool SetDrawVar(size_t pUid, void* value, size_t valueSize);

    // [[nodiscard]] ConstBufferData* GetGlobalProps() const { return _globalProps; }
    [[nodiscard]] D3D11_SHADER_DESC GetDescription() const { return _description; }
    [[nodiscard]] bool HasGlobalProps() const { return _hasGlobalProps; }
    [[nodiscard]] bool HasDrawProps() const { return _hasDrawProps; }
    [[nodiscard]] ConstBufferData* GetDrawProps() const { return _drawProps; }
    [[nodiscard]] ConstBufferData* GetMaterialPropsLookup() const { return _materialPropsLookup; }
    [[nodiscard]] std::unordered_map<size_t, D3D11_SHADER_INPUT_BIND_DESC>* GetResourcesLookup() const { return _resourcesLookup; }
#ifdef SHADER_PROGRAM_TEMPLATE_TYPE_VERTEX
    [[nodiscard]] ID3D11InputLayout* GetInputLayout() const { return _inputLayout; }
    [[nodiscard]] ID3D11VertexShader* AsID3D11() const { return _vs; }
#elif defined(SHADER_PROGRAM_TEMPLATE_TYPE_PIXEL)
    [[nodiscard]] ID3D11PixelShader* AsID3D11() const { return _ps; }
#endif

private:
    bool CreateConstBufferLookups(ID3D11Device* device);
    bool CreateConstBufferLookup(ID3D11Device* device, ConstBufferData* buffer);

private:
    wchar_t* _path = nullptr;

    #ifdef SHADER_PROGRAM_TEMPLATE_TYPE_VERTEX
    ID3D11VertexShader* _vs = nullptr;
    #elif defined(SHADER_PROGRAM_TEMPLATE_TYPE_PIXEL)
    ID3D11PixelShader* _ps = nullptr;
    #endif

    ID3D11ShaderReflection* _reflection = nullptr;
#ifdef SHADER_PROGRAM_TEMPLATE_TYPE_VERTEX
    ID3D11InputLayout*      _inputLayout = nullptr;
#endif

    // ConstBufferData*        _globalProps = nullptr;
    D3D11_SHADER_DESC       _description;
    bool                    _hasGlobalProps = false;
    bool                    _hasDrawProps = false;
    ConstBufferData*        _drawProps = nullptr;
    ConstBufferData*        _materialPropsLookup = nullptr;
    std::unordered_map<size_t, D3D11_SHADER_INPUT_BIND_DESC>* _resourcesLookup = nullptr;

};

#endif

// ======================================================
// ============= HEADER TEMPLATE END ====================
// ======================================================

// ======================================================
// =============== SOURCE TEMPLATE ======================
// ======================================================

#ifdef SHADER_PROGRAM_TEMPLATE_SOURCE

#include <d3d10shader.h>
#include <d3dcompiler.h>
#include <iostream>

SHADER_PROGRAM_TEMPLATE_CLASS_NAME::SHADER_PROGRAM_TEMPLATE_CLASS_NAME(const wchar_t* path)
{
    _path = new wchar_t[256];

    // ToDo Assert or something
    wcscpy_s(_path, 256, path);
}

SHADER_PROGRAM_TEMPLATE_CLASS_NAME::~SHADER_PROGRAM_TEMPLATE_CLASS_NAME()
{
    Release();

    delete _path;
    // delete _globalProps;
    delete _drawProps;
    delete _materialPropsLookup;
    delete _resourcesLookup;
}

void SHADER_PROGRAM_TEMPLATE_CLASS_NAME::Release()
{
#ifdef SHADER_PROGRAM_TEMPLATE_TYPE_VERTEX
    if (_vs)
    {
        _vs->Release();
        _vs = nullptr;
    }
#endif

#ifdef SHADER_PROGRAM_TEMPLATE_TYPE_PIXEL
    if (_ps)
    {
        _ps->Release();
        _ps = nullptr;
    }
#endif

    if (_reflection)
    {
        _reflection->Release();
        _reflection = nullptr;
    }

#ifdef SHADER_PROGRAM_TEMPLATE_TYPE_VERTEX
    if (_inputLayout)
    {
        _inputLayout->Release();
        _inputLayout = nullptr;
    }
#endif
}

bool SHADER_PROGRAM_TEMPLATE_CLASS_NAME::Compile(ID3D11Device* device)
{
    HRESULT result;
    ID3D10Blob* errorMessage = nullptr;
    ID3D10Blob* sBuffer;
    D3D11_INPUT_ELEMENT_DESC inputLayout[4];

#ifdef SHADER_PROGRAM_TEMPLATE_TYPE_VERTEX
    result = D3DCompileFromFile(_path, nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "VertexMain", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0,
                                &sBuffer, &errorMessage);
#endif
#ifdef SHADER_PROGRAM_TEMPLATE_TYPE_PIXEL
    result = D3DCompileFromFile(_path, nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "PixelMain", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0,
                                &sBuffer, &errorMessage);
#endif

    if(FAILED(result))
    {
        // If the shader failed to compile it should have writen something to the error message.
        if(errorMessage)
        {
            auto msg = static_cast<char*>(errorMessage->GetBufferPointer());
            std::wcerr << "Error occured while compiling a shader (" << _path << "): " << std::endl << msg << std::endl; // ToDo
        }
        // If there was nothing in the error message then it simply could not find the shader file itself.
        else
        {
            std::wcerr << "Error occured while compiling a shader (" << _path << "): " << "Missing shader file" << std::endl; // ToDo
        }

        return false;
    }

    // Create the vertex shader from the buffer.
#ifdef SHADER_PROGRAM_TEMPLATE_TYPE_VERTEX
    result = device->CreateVertexShader(sBuffer->GetBufferPointer(), sBuffer->GetBufferSize(), NULL, &_vs);
#endif
#ifdef SHADER_PROGRAM_TEMPLATE_TYPE_PIXEL
    result = device->CreatePixelShader(sBuffer->GetBufferPointer(), sBuffer->GetBufferSize(), NULL, &_ps);
#endif
    if(FAILED(result))
    {
        return false;
    }

    // Create the vertex input layout description.
    inputLayout[0].SemanticName = "POSITION";
    inputLayout[0].SemanticIndex = 0;
    inputLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
    inputLayout[0].InputSlot = 0;
    inputLayout[0].AlignedByteOffset = 0;
    inputLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
    inputLayout[0].InstanceDataStepRate = 0;

    inputLayout[1].SemanticName = "NORMAL";
    inputLayout[1].SemanticIndex = 0;
    inputLayout[1].Format = DXGI_FORMAT_R32G32B32_FLOAT;
    inputLayout[1].InputSlot = 0;
    inputLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
    inputLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
    inputLayout[1].InstanceDataStepRate = 0;

    inputLayout[2].SemanticName = "TEXCOORD";
    inputLayout[2].SemanticIndex = 0;
    inputLayout[2].Format = DXGI_FORMAT_R32G32_FLOAT;
    inputLayout[2].InputSlot = 0;
    inputLayout[2].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
    inputLayout[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
    inputLayout[2].InstanceDataStepRate = 0;

    inputLayout[3].SemanticName = "COLOR";
    inputLayout[3].SemanticIndex = 0;
    inputLayout[3].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
    inputLayout[3].InputSlot = 0;
    inputLayout[3].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
    inputLayout[3].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
    inputLayout[3].InstanceDataStepRate = 0;

    UINT inputLayoutSize = static_cast<UINT>(std::size(inputLayout));


#ifdef SHADER_PROGRAM_TEMPLATE_TYPE_VERTEX
    // Create the vertex input layout.
    result = device->CreateInputLayout(inputLayout, inputLayoutSize, sBuffer->GetBufferPointer(),
                                       sBuffer->GetBufferSize(), &_inputLayout);
    if(FAILED(result))
    {
        return false;
    }
#endif


#ifdef SHADER_PROGRAM_TEMPLATE_TYPE_VERTEX
    result = D3DReflect( sBuffer->GetBufferPointer(), sBuffer->GetBufferSize(),
            IID_ID3D11ShaderReflection, reinterpret_cast<void**>(&_reflection));
    if(FAILED(result))
    {
        return false;
    }
#endif

#ifdef SHADER_PROGRAM_TEMPLATE_TYPE_PIXEL
    result = D3DReflect( sBuffer->GetBufferPointer(), sBuffer->GetBufferSize(),
            IID_ID3D11ShaderReflection, reinterpret_cast<void**>(&_reflection));
    if(FAILED(result))
    {
        return false;
    }
#endif

    if (!CreateConstBufferLookups(device))
    {
        return false;
    }



    sBuffer->Release(); // ToDo ?
    sBuffer = nullptr;

    return true;
}

// bool SHADER_PROGRAM_TEMPLATE_CLASS_NAME::GetVariableDescription(
//     size_t cBufferId, size_t propertyId, _Out_ D3D11_SHADER_VARIABLE_DESC& desc) const
// {
//     auto cBuffer = _variablesLookup.find(cBufferId);
//     if (cBuffer == _variablesLookup.end())
//     {
//         return false;
//     }
//
//     auto property = cBuffer->second->find(propertyId);
//     if (property == cBuffer->second->end())
//     {
//         return false;
//     }
//
//     desc = property->second;
//     return true;
// }

// bool SHADER_PROGRAM_TEMPLATE_CLASS_NAME::SetVar(ConstBufferData* buffer, size_t pUid, void* value, size_t valueSize)
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

// bool SHADER_PROGRAM_TEMPLATE_CLASS_NAME::SetGlobalVar(size_t pUid, void* value, size_t valueSize)
// {
//     if (!_globalProps)
//     {
//         return false;
//     }
//
//     return _globalProps->SetVar(pUid, value, valueSize);
// }

bool SHADER_PROGRAM_TEMPLATE_CLASS_NAME::SetDrawVar(size_t pUid, void* value, size_t valueSize)
{
    if (!_drawProps)
    {
        return false;
    }

    return _drawProps->SetVar(pUid, value, valueSize);
}

bool SHADER_PROGRAM_TEMPLATE_CLASS_NAME::UploadBuffer(ID3D11DeviceContext* deviceContext, ConstBufferData* buffer)
{
    if (!buffer->IsDirty())
    {
        return false; // ToDo what state should be returned?
    }

    HRESULT result;
    D3D11_MAPPED_SUBRESOURCE mappedResource;

    // Lock the constant buffer so it can be written to.
    result = deviceContext->Map(buffer->bPtr, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
    if(FAILED(result))
    {
        return false;
    }

    memcpy(mappedResource.pData, buffer->vPtr, buffer->size);

    // Unlock the constant buffer.
    deviceContext->Unmap(buffer->bPtr, 0);

    buffer->UnmarkDirty();

    return true;
}

// bool SHADER_PROGRAM_TEMPLATE_CLASS_NAME::UploadDrawBuffer(ID3D11DeviceContext *deviceContext)
// {
//     if (_drawProps)
//         return UploadBuffer(deviceContext, _drawProps);
// }

bool SHADER_PROGRAM_TEMPLATE_CLASS_NAME::CreateConstBufferLookup(ID3D11Device* device, ConstBufferData* buffer)
{
    HRESULT result;
    ID3D11ShaderReflectionConstantBuffer* cBuffer = nullptr;
    D3D11_SHADER_BUFFER_DESC cBufferDesc;
    D3D11_SHADER_VARIABLE_DESC varDesc;

    ID3D11ShaderReflectionVariable* var;

    // Get buffer
    cBuffer = _reflection->GetConstantBufferByIndex(buffer->id);

    // Get buffer description
    result = cBuffer->GetDesc(&cBufferDesc);
    if (FAILED(result))
    {
        return false;
    }

    // Iterate through all buffer variables
    for (UINT i = 0; i < cBufferDesc.Variables; i++)
    {
        // Get variable
        var = cBuffer->GetVariableByIndex(i);

        // Get variable description
        result = var->GetDesc(&varDesc);
        if (FAILED(result))
        {
            return false;
        }

        buffer->vDesc.emplace(ShaderUtils::PropertyToID(varDesc.Name), varDesc);
    }

    // D3D11_BUFFER_DESC bufferDesc;
    //
    // // Setup the description of the dynamic matrix constant buffer that is in the vertex shader.
    // bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
    // bufferDesc.ByteWidth = buffer->description.Size;
    // bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    // bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    // bufferDesc.MiscFlags = 0;
    // bufferDesc.StructureByteStride = 0;

    // result = device->CreateBuffer(&bufferDesc, nullptr, &buffer->bPtr); // ToDo initial data
    // if(FAILED(result))
    // {
    //     return false;
    // }

    return true;
}

bool SHADER_PROGRAM_TEMPLATE_CLASS_NAME::CreateConstBufferLookups(ID3D11Device* device)
{
    if (FAILED(_reflection->GetDesc(&_description)))
    {
        return false;
    }

    D3D11_SHADER_BUFFER_DESC bufferDesc;

    for (UINT cBufferIndex = 0; cBufferIndex < _description.ConstantBuffers; cBufferIndex++)
    {
        auto cBuffer = _reflection->GetConstantBufferByIndex(cBufferIndex);

        if (FAILED(cBuffer->GetDesc(&bufferDesc)))
        {
            return false;
        }

        size_t bufferUid = ShaderUtils::PropertyToID(bufferDesc.Name);
        ConstBufferData* buffer = nullptr;

        if (bufferUid == ShaderUtils::PropertyToID("GlobalProperties"))
        {
            _hasGlobalProps = true;
        }
        else if (bufferUid == ShaderUtils::PropertyToID("DrawProperties"))
        {
            _hasDrawProps = true;
        }
        else if (bufferUid == ShaderUtils::PropertyToID("MaterialProperties"))
        {
            _materialPropsLookup = new ConstBufferData{device, cBufferIndex, bufferUid, bufferDesc, 0};
            buffer = _materialPropsLookup;
        }

        if (buffer == nullptr)
        {
            continue;
        }

        if (!CreateConstBufferLookup(device, buffer))
        {
            return false;
        }
    }

    D3D11_SHADER_INPUT_BIND_DESC bResDesc;



    _resourcesLookup = new std::unordered_map<unsigned long long, _D3D11_SHADER_INPUT_BIND_DESC>{};

    // Create bound resources lookup
    for (UINT bResIndex = 0; bResIndex < _description.BoundResources; bResIndex++)
    {
        if (FAILED(_reflection->GetResourceBindingDesc(bResIndex, &bResDesc)))
        {
            std::cerr << "Failed to get resource binding description" << std::endl;
            return false;
        }

        // ToDo
        if (bResDesc.Type != D3D_SIT_TEXTURE && bResDesc.Type != D3D_SIT_SAMPLER)
        {
            continue;
        }

        _resourcesLookup->emplace(ShaderUtils::PropertyToID(bResDesc.Name), bResDesc);
    }

    return true;
}

#endif

// ======================================================
// ============= SOURCE TEMPLATE END ====================
// ======================================================

// Undefine all defines

#undef SHADER_PROGRAM_TEMPLATE_SOURCE
#undef SHADER_PROGRAM_TEMPLATE_HEADER
#undef SHADER_PROGRAM_TEMPLATE_CLASS_NAME
#undef SHADER_PROGRAM_TEMPLATE_TYPE_VERTEX
#undef SHADER_PROGRAM_TEMPLATE_TYPE_PIXEL