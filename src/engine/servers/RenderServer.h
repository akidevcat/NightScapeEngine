#ifndef RENDER_SERVER_H
#define RENDER_SERVER_H


// #pragma comment(lib, "d3dcompiler.lib")

#include <d3dcompiler.h>
#include <dxgi.h>
#include <d3d11.h>
#include <directxmath.h>

#include "../data/Mesh.h"
#include "../entity/Camera.h"
#include "../render/Material.h"
using namespace DirectX;

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
    void PipelineSetMesh(Mesh* mesh);
    void PipelineSetMaterial(Material* material);
    void PipelineDrawIndexed(Mesh* mesh);

    // ===== Render Methods =====
    void BeginScene(XMFLOAT4 color);
    void EndScene();
    void DrawMesh(Mesh* mesh, Material* material, XMMATRIX matrix, Camera* camera);

// Property Accessors
    bool                        GetFullscreenState() const { return _isFullscreen; }
    void                        SetFullscreenState(bool state) { _isFullscreen = state; }
    ID3D11Device*               GetDevice() const { return _device; }
    ID3D11DeviceContext*        GetDeviceContext() const { return _deviceContext; }

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

    XMMATRIX                    _projectionMatrix;
    XMMATRIX                    _worldMatrix;
    XMMATRIX                    _orthoMatrix;
};

#endif //RENDER_SERVER_H