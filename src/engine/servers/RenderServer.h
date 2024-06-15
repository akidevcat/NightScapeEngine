#ifndef RENDER_SERVER_H
#define RENDER_SERVER_H

#include <dxgi.h>
#include <d3d11.h>
#include <directxmath.h>

#include "BaseServer.h"
#include "../data/Mesh.h"
#include "../entity/Camera.h"
#include "../render/DrawProperties.h"
#include "../render/GlobalProperties.h"
#include "../render/Material.h"
#include "../render/RenderTexture.h"
#include "../data/LightsProperties.h"

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")

namespace NSE
{
    class RenderServer : public BaseServer<RenderServer>
    {
    public:
        RenderServer();
        ~RenderServer();
        bool Initialize(int screenWidth, int screenHeight, HWND hwnd);
        void Shutdown();

        // ===== Pipeline Methods =====
        void PipelineSetCamera(const NSE_Camera& camera);
        void PipelineSetModelMatrix(const DirectX::XMMATRIX& matrix);
        void PipelineSetMesh(const NSE_Mesh& mesh);
        void PipelineSetMaterial(const NSE_Material& material);
        void PipelineDrawIndexed(const NSE_Mesh& mesh);
        void PipelineSetRenderTargets(ID3D11RenderTargetView* colorTarget, ID3D11DepthStencilView* depthTarget, const D3D11_VIEWPORT& viewport);
        void PipelineSetRenderTargets(const NSE_RenderTexture& colorTarget, const NSE_RenderTexture& depthTarget);
        // void PipelineResetRenderTarget();

        void ClearRenderTextureColor(const NSE_RenderTexture& target, DirectX::XMFLOAT4 color) const;
        void ClearRenderTextureDepth(const NSE_RenderTexture& target, float depth) const;

        // void PipelineClearRenderTexture(const NSE_RenderTexture& target, bool clearColor, bool clearDepth, DirectX::XMFLOAT4 color, float depth);
        void PipelineClearRenderTarget(bool clearColor, bool clearDepth, DirectX::XMFLOAT4 color, float depth);

        // ===== Render Methods =====
        void Present();
        // void ClearRenderTarget(const NSE_RenderTexture& target, DirectX::XMFLOAT4 color);
        void DrawMesh(const NSE_Mesh& mesh, const NSE_Material& material, const DirectX::XMMATRIX& matrix, const NSE_Camera& camera, size_t objectID = 0);

        // Property Accessors
        [[nodiscard]] LightsProperties*           GetLightsProperties() const { return GetLightsPropertiesBuffer()->GetBufferData()->As<LightsProperties>(); }
        [[nodiscard]] NSE_ConstantBuffer          GetGlobalPropertiesBuffer() const { return _globalPropertiesBuffer; }
        [[nodiscard]] NSE_ConstantBuffer          GetDrawPropertiesBuffer() const { return _drawPropertiesBuffer; }
        [[nodiscard]] NSE_ConstantBuffer          GetLightsPropertiesBuffer() const { return _lightsPropertiesBuffer; }
        [[nodiscard]] GlobalProperties*           GetGlobalProperties() const { return _globalPropertiesBuffer->GetBufferData()->As<GlobalProperties>(); }
        [[nodiscard]] bool                        GetFullscreenState() const { return _isFullscreen; }
        void                                      SetFullscreenState(bool state) { _isFullscreen = state; }
        [[nodiscard]] ID3D11Device*               GetDevice() const { return _device; }
        [[nodiscard]] ID3D11DeviceContext*        GetDeviceContext() const { return _deviceContext; }
        [[nodiscard]] ID3D11SamplerState*         GetDefaultPointSampler() const { return _defaultPointSampler; }
        [[nodiscard]] ID3D11SamplerState*         GetDefaultLinearSampler() const { return _defaultLinearSampler; }
        [[nodiscard]] ID3D11RenderTargetView*     GetColorTargetView() const { return _renderTargetView; }
        [[nodiscard]] ID3D11DepthStencilView*     GetDepthTargetView() const { return _depthStencilView; }
        [[nodiscard]] D3D11_VIEWPORT              GetViewport() const { return _viewport; }
        [[nodiscard]] NSE_Mesh                    GetPrimitiveQuadMesh() const { return _primitiveQuadMesh; }

        [[nodiscard]] NSE_BlendState              GetBlendStateOpaque() const { return _defaultOpaqueBlendState; }
        [[nodiscard]] NSE_BlendState              GetBlendStateAdditive() const { return _defaultAdditiveBlendState; }
        [[nodiscard]] NSE_BlendState              GetBlendStateTransparent() const { return _defaultTransparentBlendState; }

    private:

        bool                        _currentStateDepthWrite = true;
        NSE_BlendState              _currentStateBlend = nullptr;
        NSE_Mesh                    _currentStateMesh = nullptr;
        D3D11_PRIMITIVE_TOPOLOGY    _currentStatePrimitiveTopology = D3D_PRIMITIVE_TOPOLOGY_UNDEFINED;
        ID3D11VertexShader*         _currentStateVertexShader = nullptr;
        ID3D11PixelShader*          _currentStatePixelShader = nullptr;
        ID3D11RenderTargetView*     _currentRenderTargetView = nullptr;
        ID3D11DepthStencilView*     _currentDepthStencilView = nullptr;

        // ToDo primitives
        NSE_Mesh                    _primitiveQuadMesh = nullptr;

        bool                        _isFullscreen = false;
        bool                        _isVSyncEnabled = false;

        int                         _videoCardMemory;
        char                        _videoCardDescription[128];

        IDXGISwapChain*             _swapChain = nullptr;
        ID3D11Device*               _device = nullptr;
        ID3D11DeviceContext*        _deviceContext = nullptr;
        ID3D11RenderTargetView*     _renderTargetView = nullptr;
        ID3D11Texture2D*            _depthStencilBuffer = nullptr;
        ID3D11DepthStencilState*    _depthStencilState = nullptr;
        ID3D11DepthStencilState*    _depthDisabledStencilState = nullptr;
        ID3D11DepthStencilView*     _depthStencilView = nullptr;
        ID3D11RasterizerState*      _rasterState = nullptr;
        D3D11_VIEWPORT              _viewport;
        ID3D11BlendState*           _alphaEnableBlendingState = nullptr;
        ID3D11BlendState*           _alphaDisableBlendingState = nullptr;
        ID3D11SamplerState*         _defaultPointSampler = nullptr;
        ID3D11SamplerState*         _defaultLinearSampler = nullptr;
        NSE_BlendState              _defaultOpaqueBlendState = nullptr;
        NSE_BlendState              _defaultAdditiveBlendState = nullptr;
        NSE_BlendState              _defaultTransparentBlendState = nullptr;

        NSE_Shader                  _errorShader = nullptr;
        NSE_Material                _errorMaterial = nullptr;

        NSE_ConstantBuffer          _globalPropertiesBuffer = nullptr;
        NSE_ConstantBuffer          _drawPropertiesBuffer = nullptr;
        NSE_ConstantBuffer          _lightsPropertiesBuffer = nullptr;

        ShaderInputsData*           _globalShaderInputs = nullptr;
    };
}

#endif //RENDER_SERVER_H