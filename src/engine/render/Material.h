#ifndef MATERIAL_H
#define MATERIAL_H

#include <directxmath.h>

#include "BlendState.h"
#include "GraphicsBuffer.h"
#include "../obj_ptr.h"
#include "Shader.h"
#include "ShaderInputsData.h"
#include "Texture.h"
#include "../data/ShaderDepthState.h"
#include "../data/SpriteTexture.h"
#include "../entity/Object.h"
#include "../math/Math.h"

#define NSE_Material obj_ptr<NSE::Material>

namespace NSE
{


    class Material : public Object
    {
    public:
        enum RenderQueue : unsigned short
        {
            RENDER_QUEUE_BACKGROUND = 10000,
            RENDER_QUEUE_OPAQUE = 20000,
            RENDER_QUEUE_TRANSPARENT = 30000,
            RENDER_QUEUE_OVERLAY = 40000
        };
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
        void SetFloat2(size_t nameID, float2 value) const;
        void SetFloat3(size_t nameID, float3 value) const;
        void SetFloat4(size_t nameID, float4 value) const;
        void SetVector(size_t nameID, xmvector value) const;
        void SetColor(size_t nameID, xmvector value) const;
        void SetColor(size_t nameID, float4 value) const;
        void SetMatrix(size_t nameID, xmmatrix value) const;
        void SetConstantBuffer(size_t nameID, const NSE_GraphicsBuffer& buffer) const;
        void SetBuffer(size_t nameID, const NSE_GraphicsBuffer& buffer) const;
        void SetTexture(size_t nameID, const NSE_Texture& texture) const;
        // void SetSpriteTexture(size_t nameID, const SpriteTexture& texture) const;

        void SetBlendState(const NSE_BlendState& state);
        void SetDepthWrite(ShaderDepthState state) { _depthReadWriteState = state; }
        [[nodiscard]] NSE_BlendState GetBlendState() const { return _blendState; }
        [[nodiscard]] ShaderDepthState GetDepthWrite() const { return _depthReadWriteState; }

        void MakeOpaque();
        void MakeTransparent();
        void MakeAdditive();
        void MakeInvert();

    public:
        unsigned short renderQueue = RENDER_QUEUE_OPAQUE;

    private:
        NSE_Shader       _shader = nullptr;
        NSE_BlendState   _blendState = nullptr;
        ShaderDepthState _depthReadWriteState = ShaderDepthState::ReadWrite;

        NSE_GraphicsBuffer _materialPropertiesBuffer = nullptr;
        ShaderInputsData*   _vsInputs = nullptr;
        ShaderInputsData*   _psInputs = nullptr;
    };
}

#define NSE_Material obj_ptr<NSE::Material>

#endif //MATERIAL_H
