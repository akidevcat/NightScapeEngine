#ifndef RENDER_SERVER_H
#define RENDER_SERVER_H

#include <dxgi.h>
#include <d3d11.h>
#include <directxmath.h>

#include "../data/Mesh.h"
#include "../entity/Camera.h"
#include "../render/DrawProperties.h"
#include "../render/GlobalProperties.h"
#include "../render/Material.h"
#include "../render/RenderTexture.h"

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")

class RenderServer
{
public:
    RenderServer();
    ~RenderServer();
    bool Initialize(int screenWidth, int screenHeight, HWND hwnd);
    void Shutdown();

    // ===== Pipeline Methods =====
    void PipelineMarkGlobalPropertiesDirty();
    void PipelineSetCamera(Camera* camera);
    void PipelineSetModelMatrix(const DirectX::XMMATRIX& matrix);
    void PipelineSetMesh(Mesh* mesh);
    void PipelineSetMaterial(Material* material);
    void PipelineDrawIndexed(Mesh* mesh);
    void PipelineSetRenderTarget(RenderTexture* target);
    void PipelineResetRenderTarget();

    // ===== Render Methods =====
    void BeginScene(DirectX::XMFLOAT4 color);
    void EndScene();
    void ClearRenderTarget(RenderTexture* target, DirectX::XMFLOAT4 color);
    void DrawMesh(Mesh* mesh, Material* material, const DirectX::XMMATRIX &matrix, Camera* camera);

    // Property Accessors
    [[nodiscard]] GlobalProperties*           GetGlobalProperties() const { return _globalProperties; }
    [[nodiscard]] bool                        GetFullscreenState() const { return _isFullscreen; }
                  void                        SetFullscreenState(bool state) { _isFullscreen = state; }
    [[nodiscard]] ID3D11Device*               GetDevice() const { return _device; }
    [[nodiscard]] ID3D11DeviceContext*        GetDeviceContext() const { return _deviceContext; }
    [[nodiscard]] ID3D11SamplerState*         GetDefaultPointSampler() const { return _defaultPointSampler; }
    [[nodiscard]] ID3D11SamplerState*         GetDefaultLinearSampler() const { return _defaultLinearSampler; }

private:

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

    Shader*                     _errorShader = nullptr;
    Material*                   _errorMaterial = nullptr;

    GlobalProperties*           _globalProperties = new GlobalProperties{};
    ConstBufferData*            _globalPropertiesBuffer = nullptr;
    DrawProperties*             _drawProperties = new DrawProperties{};
    ConstBufferData*            _drawPropertiesBuffer = nullptr;
};

#endif //RENDER_SERVER_H