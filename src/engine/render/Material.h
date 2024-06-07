#ifndef MATERIAL_H
#define MATERIAL_H

#include <directxmath.h>

#include "ConstantBufferData.h"
#include "../obj_ptr.h"
#include "Shader.h"
#include "ShaderInputsData.h"
#include "../entity/Object.h"

#define NSE_Material obj_ptr<NSE::Material>

namespace NSE
{
    enum MATERIAL_RENDER_QUEUE : short
    {
        MATERIAL_RENDER_QUEUE_BACKGROUND = -10000,
        MATERIAL_RENDER_QUEUE_OPAQUE = 0,
        MATERIAL_RENDER_QUEUE_TRANSPARENT = 10000,
        MATERIAL_RENDER_QUEUE_OVERLAY = 20000
    };

    class Material : public Object
    {
    public:
        explicit Material(const NSE_Shader& shader);
        ~Material() override;

        void Upload();
        // bool UploadMaterialProperties(ID3D11DeviceContext *context);
        // void UploadDrawProperties(ConstBufferData* drawProps);
        // void UploadGlobalProperties(ConstBufferData* globalProps);
        // void UploadAllProperties(ID3D11DeviceContext *context, ConstBufferData* globalProps, ConstBufferData* drawProps);
        void EnumerateBuffers(_Out_ ID3D11Buffer* vsBuffers[3], _Out_ int& vsBuffersLength,
                              _Out_ ID3D11Buffer* psBuffers[3], _Out_ int& psBuffersLength,
                              ConstantBuffer* globalProps, ConstantBuffer* drawProps) const;
        void EnumerateVertexBuffers(_Out_ ID3D11Buffer* vsBuffers[3], _Out_ int& vsBuffersLength,
                                    const ConstantBuffer* globalPropertiesBuffer, const ConstantBuffer* drawPropertiesBuffer) const;
        void EnumeratePixelBuffers(_Out_ ID3D11Buffer* psBuffers[3], _Out_ int& psBuffersLength,
                                   const ConstantBuffer* globalPropertiesBuffer, const ConstantBuffer* drawPropertiesBuffer) const;

    public:
        [[nodiscard]] NSE_Shader GetShader() const { return _shader; }
        [[nodiscard]] ShaderInputsData* GetVSInputs() const { return _vsInputs; }
        [[nodiscard]] ShaderInputsData* GetPSInputs() const { return _psInputs; }
        [[nodiscard]] bool HasVSMaterialProps() const { return _shader->GetVertexShader()->HasMaterialProps(); }
        [[nodiscard]] bool HasPSMaterialProps() const { return _shader->GetPixelShader()->HasMaterialProps(); }

        void SetVar(size_t nameID, void* valuePtr, size_t valueSize) const;
        void SetFloat(size_t nameID, float value) const;
        void SetInt(size_t nameID, int value) const;
        void SetUnsignedInt(size_t nameID, uint32_t value) const;
        void SetVector(size_t nameID, DirectX::XMVECTOR value) const;
        void SetColor(size_t nameID, DirectX::XMVECTOR value) const;
        void SetMatrix(size_t nameID, DirectX::XMMATRIX value) const;

        void SetBlendState(const D3D11_BLEND_DESC& description);
        void SetBlendStateTransparency();
        void SetBlendStateAdditive();
        void SetBlendStateOpaque();
        [[nodiscard]] ID3D11BlendState* GetBlendState() const { return _blendState; }
        [[nodiscard]] bool GetDepthWrite() const { return _depthWriteEnabled; }

        // [[nodiscard]] ConstBufferData* GetVSMaterialProps() const { return _vsMaterialProps; }
        // [[nodiscard]] ConstBufferData* GetPSMaterialProps() const { return _psMaterialProps; }

    public:
        int               renderQueue = MATERIAL_RENDER_QUEUE_OPAQUE;

    private:
        NSE_Shader       _shader = nullptr;
        ID3D11BlendState* _blendState = nullptr;
        bool                _depthWriteEnabled = true;

        ConstantBufferData* _vsMaterialPropertiesBuffer = nullptr;
        ConstantBufferData* _psMaterialPropertiesBuffer = nullptr;
        ShaderInputsData*   _vsInputs = nullptr;
        ShaderInputsData*   _psInputs = nullptr;
    };
}

#define NSE_Material obj_ptr<NSE::Material>

#endif //MATERIAL_H
