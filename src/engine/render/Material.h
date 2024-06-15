#ifndef MATERIAL_H
#define MATERIAL_H

#include <directxmath.h>

#include "BlendState.h"
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
        MATERIAL_RENDER_QUEUE_BACKGROUND = -1000,
        MATERIAL_RENDER_QUEUE_OPAQUE = 0,
        MATERIAL_RENDER_QUEUE_TRANSPARENT = 1000,
        MATERIAL_RENDER_QUEUE_OVERLAY = 2000
    };

    class Material : public Object
    {
    public:
        explicit Material(const NSE_Shader& shader);
        ~Material() override;

        void Upload();

        void EnumerateVertexConstantBuffers(
            _Out_ ID3D11Buffer* vsBuffers[D3D11_COMMONSHADER_CONSTANT_BUFFER_HW_SLOT_COUNT],
            _Out_ int& vsBuffersLength) const;

        void EnumeratePixelConstantBuffers(
            _Out_ ID3D11Buffer* psBuffers[D3D11_COMMONSHADER_CONSTANT_BUFFER_HW_SLOT_COUNT],
            _Out_ int& psBuffersLength) const;

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
        void SetConstantBuffer(size_t nameID, const NSE_ConstantBuffer& buffer) const;
        void SetConstantBuffer(const NSE_ConstantBuffer& buffer) const;

        void SetBlendState(const NSE_BlendState& state);
        void SetDepthWrite(bool state) { _depthWriteEnabled = state; }
        [[nodiscard]] NSE_BlendState GetBlendState() const { return _blendState; }
        [[nodiscard]] bool GetDepthWrite() const { return _depthWriteEnabled; }

    public:
        int               renderQueue = MATERIAL_RENDER_QUEUE_OPAQUE;

    private:
        NSE_Shader       _shader = nullptr;
        NSE_BlendState   _blendState = nullptr;
        bool             _depthWriteEnabled = true;

        ConstantBufferData* _vsMaterialPropertiesBuffer = nullptr;
        ConstantBufferData* _psMaterialPropertiesBuffer = nullptr;
        ShaderInputsData*   _vsInputs = nullptr;
        ShaderInputsData*   _psInputs = nullptr;
    };
}

#define NSE_Material obj_ptr<NSE::Material>

#endif //MATERIAL_H
