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

#include <cassert>
#include "ConstantBuffer.h"
#include "ConstantBufferData.h"
#include "ShaderInputsDescription.h"
#include "../servers/ObjectServer.h"

#ifdef SHADER_PROGRAM_TEMPLATE_HEADER

#include "ShaderUtils.h"
#include <d3d11.h>
#include <d3d11shader.h>
#include <unordered_map>

namespace NSE
{
    class SHADER_PROGRAM_TEMPLATE_CLASS_NAME
    {
    public:
        explicit SHADER_PROGRAM_TEMPLATE_CLASS_NAME(const wchar_t* path);
        ~SHADER_PROGRAM_TEMPLATE_CLASS_NAME();

        void Release();
        bool Compile(ID3D11Device* device);

        // bool UploadBuffer(ID3D11DeviceContext* deviceContext, ConstBufferData* buffer);

        // bool UploadDrawBuffer(ID3D11DeviceContext* deviceContext);
        // bool UploadGlobalBuffer(ID3D11DeviceContext* deviceContext);

        // bool SetGlobalVar(size_t pUid, void* value, size_t valueSize);
        // bool SetDrawVar(size_t pUid, void* value, size_t valueSize);

        // [[nodiscard]] ConstBufferData* GetGlobalProps() const { return _globalProps; }

        void UploadMaterialProperties(ConstantBufferData* data);

        [[nodiscard]] D3D11_SHADER_DESC        GetDescription() const { return _description; }
        [[nodiscard]] ShaderInputsDescription* GetInputsDescription() const { return _inputsDescription; }
        [[nodiscard]] NSE_ConstantBuffer       GetMaterialPropertiesBuffer() const { return _materialPropertiesBuffer; }
        [[nodiscard]] UINT                     GetMaterialPropertiesBufferSize() const { return _materialPropertiesBuffer->GetDescription()->GetDescription().Size; }
        [[nodiscard]] bool                     HasMaterialProps() const { return _hasMaterialProps; }
        [[nodiscard]] bool                     HasGlobalProps() const { return _hasGlobalProps; }
        [[nodiscard]] bool                     HasDrawProps() const { return _hasDrawProps; }

        // [[nodiscard]] ConstBufferData* GetDrawProps() const { return _drawProps; }
        // [[nodiscard]] ConstBufferData* GetMaterialPropsLookup() const { return _materialPropsLookup; }
        // [[nodiscard]] std::unordered_map<size_t, D3D11_SHADER_INPUT_BIND_DESC>* GetResourcesLookup() const { return _resourcesLookup; }
#ifdef SHADER_PROGRAM_TEMPLATE_TYPE_VERTEX
        [[nodiscard]] ID3D11InputLayout* GetInputLayout() const { return _inputLayout; }
        [[nodiscard]] ID3D11VertexShader* AsID3D11() const { return _vs; }
#elif defined(SHADER_PROGRAM_TEMPLATE_TYPE_PIXEL)
        [[nodiscard]] ID3D11PixelShader* AsID3D11() const { return _ps; }
#endif

    private:
        // bool CreateConstBufferLookups(ID3D11Device* device);
        // bool CreateConstBufferLookup(ID3D11Device* device, ConstBufferData* buffer);
        void Reflect();
        void CreateMaterialPropertiesBuffer();

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

        NSE_ConstantBuffer         _materialPropertiesBuffer = nullptr;
        D3D11_SHADER_DESC          _description;
        ShaderInputsDescription*   _inputsDescription;
        bool                       _hasMaterialProps = false;
        bool                       _hasGlobalProps = false;
        bool                       _hasDrawProps = false;

        // ConstBufferData*        _drawProps = nullptr;
        // ConstBufferData*        _materialPropsLookup = nullptr;
        // std::unordered_map<size_t, D3D11_SHADER_INPUT_BIND_DESC>* _resourcesLookup = nullptr;

    };
}

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

namespace NSE
{
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
        // delete _materialPropertiesBuffer;
        delete _inputsDescription;

        // delete _globalProps;
        // delete _drawProps;
        // delete _materialPropsLookup;
        // delete _resourcesLookup;
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

        DestroyObject(_materialPropertiesBuffer);
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

        Reflect();
        CreateMaterialPropertiesBuffer();

        sBuffer->Release(); // ToDo ?
        sBuffer = nullptr;

        return true;
    }

    void SHADER_PROGRAM_TEMPLATE_CLASS_NAME::UploadMaterialProperties(ConstantBufferData* data)
    {
        if (!_materialPropertiesBuffer)
        {
            return;
        }

        _materialPropertiesBuffer->UploadData(data);
    }

    void SHADER_PROGRAM_TEMPLATE_CLASS_NAME::Reflect()
    {
        assert(("Failed to reflect shader description", SUCCEEDED(_reflection->GetDesc(&_description))));

        _inputsDescription = new ShaderInputsDescription{_reflection};

        D3D11_SHADER_INPUT_BIND_DESC bindDesc;

        _hasGlobalProps = _inputsDescription->GetDescription(ShaderUtils::PropertyToID("GlobalProperties"), bindDesc);
        _hasDrawProps = _inputsDescription->GetDescription(ShaderUtils::PropertyToID("DrawProperties"), bindDesc);
    }

    void SHADER_PROGRAM_TEMPLATE_CLASS_NAME::CreateMaterialPropertiesBuffer()
    {
        D3D11_SHADER_INPUT_BIND_DESC desc;
        size_t nameID = ShaderUtils::PropertyToID("$Globals");

        if (_inputsDescription->GetDescription(nameID, desc))
        {
            _materialPropertiesBuffer = CreateObject<ConstantBuffer>(nameID);
            _materialPropertiesBuffer->Reflect(_reflection, desc.BindPoint, true);
            _hasMaterialProps = true;
        }
    }
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