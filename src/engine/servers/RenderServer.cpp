#include "RenderServer.h"

#include "../render/GlobalProperties.h"

RenderServer::RenderServer()
{

}

RenderServer::~RenderServer()
{
	delete _errorShader;
	delete _errorMaterial;

	delete _globalProperties;
	delete _globalPropertiesBuffer;
	delete _drawProperties;
	delete _drawPropertiesBuffer;
}

bool RenderServer::Initialize(int screenWidth, int screenHeight, HWND hwnd)
{
    HRESULT result;

    // Create a DirectX graphics interface factory.
	IDXGIFactory* factory;
    result = CreateDXGIFactory(__uuidof(IDXGIFactory), reinterpret_cast<void **>(&factory));
    if(FAILED(result))
    {
        return false;
    }

    // Use the factory to create an adapter for the primary graphics interface (video card).
	IDXGIAdapter* adapter;
    result = factory->EnumAdapters(0, &adapter);
    if(FAILED(result))
    {
        return false;
    }

    // Enumerate the primary adapter output (monitor).
	IDXGIOutput* adapterOutput;
    result = adapter->EnumOutputs(0, &adapterOutput);
    if(FAILED(result))
    {
        return false;
    }

    // Get the number of modes that fit the DXGI_FORMAT_R8G8B8A8_UNORM display format for the adapter output (monitor).
	unsigned int numModes;
    result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, nullptr);
    if(FAILED(result))
    {
        return false;
    }

    // Create a list to hold all the possible display modes for this monitor/video card combination.
	DXGI_MODE_DESC* displayModeList = new DXGI_MODE_DESC[numModes];
    if(!displayModeList)
    {
        return false;
    }

    // Now fill the display mode list structures.
    result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, displayModeList);
    if(FAILED(result))
    {
        return false;
    }

    unsigned int refreshRateNum;
    unsigned int refreshRateDenum;

    // Now go through all the display modes and find the one that matches the screen width and height.
    // When a match is found store the numerator and denominator of the refresh rate for that monitor.
    for (unsigned i = 0; i < numModes; i++)
    {
        if(displayModeList[i].Width == (unsigned int)screenWidth)
        {
            if(displayModeList[i].Height == (unsigned int)screenHeight)
            {
                refreshRateNum = displayModeList[i].RefreshRate.Numerator;
                refreshRateDenum = displayModeList[i].RefreshRate.Denominator;
            }
        }
    }

    // Get the adapter (video card) description.
	DXGI_ADAPTER_DESC adapterDesc;
    result = adapter->GetDesc(&adapterDesc);
    if(FAILED(result))
    {
        return false;
    }

    // Store the dedicated video card memory in megabytes.
    _videoCardMemory = (int)(adapterDesc.DedicatedVideoMemory / 1024 / 1024);

    // Convert the name of the video card to a character array and store it.
	size_t stringLength;
    int error = wcstombs_s(&stringLength, _videoCardDescription, 128, adapterDesc.Description, 128);
    if(error != 0)
    {
        return false;
    }

    // Release the display mode list.
    delete [] displayModeList;
    displayModeList = 0;

    // Release the adapter output.
	adapterOutput->Release();
	adapterOutput = 0;

	// Release the adapter.
	adapter->Release();
	adapter = 0;

	// Release the factory.
	factory->Release();
	factory = 0;

	// Initialize the swap chain description.
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
    ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));

	// Set to a single back buffer.
    swapChainDesc.BufferCount = 1;

	// Set the width and height of the back buffer.
    swapChainDesc.BufferDesc.Width = screenWidth;
    swapChainDesc.BufferDesc.Height = screenHeight;

	// Set regular 32-bit surface for the back buffer.
    swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	// Set the refresh rate of the back buffer.
	if(_isVSyncEnabled)
	{
	    swapChainDesc.BufferDesc.RefreshRate.Numerator = refreshRateNum;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = refreshRateDenum;
	}
	else
	{
	    swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	}

	// Set the usage of the back buffer.
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

	// Set the handle for the window to render to.
    swapChainDesc.OutputWindow = hwnd;

	// Turn multisampling off.
    swapChainDesc.SampleDesc.Count = 1;
    swapChainDesc.SampleDesc.Quality = 0;

	// Set to full screen or windowed mode.
	if(_isFullscreen)
	{
		swapChainDesc.Windowed = false;
	}
	else
	{
		swapChainDesc.Windowed = true;
	}

	// Set the scan line ordering and scaling to unspecified.
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	// Discard the back buffer contents after presenting.
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	// Don't set the advanced flags.
	swapChainDesc.Flags = 0;

	// Set the feature level to DirectX 11.
	D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_0;

	// Create the swap chain, Direct3D device, and Direct3D device context.
	result = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, &featureLevel, 1,
										   D3D11_SDK_VERSION, &swapChainDesc, &_swapChain, &_device, NULL, &_deviceContext);

	if(FAILED(result))
	{
		return false;
	}

	// Get the pointer to the back buffer.
	ID3D11Texture2D* backBufferPtr;
	result = _swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBufferPtr);
	if(FAILED(result))
	{
		return false;
	}

	// Create the render target view with the back buffer pointer.
	result = _device->CreateRenderTargetView(backBufferPtr, NULL, &_renderTargetView);
	if(FAILED(result))
	{
		return false;
	}

	// Release pointer to the back buffer as we no longer need it.
	backBufferPtr->Release();
	backBufferPtr = 0;

	// Initialize the description of the depth buffer.
	D3D11_TEXTURE2D_DESC depthBufferDesc;
	ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));

	// Set up the description of the depth buffer.
	depthBufferDesc.Width = screenWidth;
	depthBufferDesc.Height = screenHeight;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;

	// Create the texture for the depth buffer using the filled out description.
	result = _device->CreateTexture2D(&depthBufferDesc, NULL, &_depthStencilBuffer);
	if(FAILED(result))
	{
		return false;
	}

	// Initialize the description of the stencil state.
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

	// Set up the description of the stencil state.
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;

	// Stencil operations if pixel is front-facing.
	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Stencil operations if pixel is back-facing.
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Create the depth stencil state.

	result = _device->CreateDepthStencilState(&depthStencilDesc, &_depthStencilState);
	if(FAILED(result))
	{
		return false;
	}

	// Set the depth stencil state.
	_deviceContext->OMSetDepthStencilState(_depthStencilState, 1);

	// Initialize the depth stencil view.
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

	// Set up the depth stencil view description.
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	// Create the depth stencil view.
	result = _device->CreateDepthStencilView(_depthStencilBuffer, &depthStencilViewDesc, &_depthStencilView);
	if(FAILED(result))
	{
		return false;
	}

	// Bind the render target view and depth stencil buffer to the output render pipeline.
	_deviceContext->OMSetRenderTargets(1, &_renderTargetView, _depthStencilView);

	// Setup the raster description which will determine how and what polygons will be drawn.
	D3D11_RASTERIZER_DESC rasterDesc;
	rasterDesc.AntialiasedLineEnable = false;
	rasterDesc.CullMode = D3D11_CULL_BACK;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = true;
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.FrontCounterClockwise = false;
	rasterDesc.MultisampleEnable = false;
	rasterDesc.ScissorEnable = false;
	rasterDesc.SlopeScaledDepthBias = 0.0f;

	// Create the rasterizer state from the description we just filled out.
	result = _device->CreateRasterizerState(&rasterDesc, &_rasterState);
	if(FAILED(result))
	{
		return false;
	}

	// Now set the rasterizer state.
	_deviceContext->RSSetState(_rasterState);

	// Setup the viewport for rendering.
    _viewport.Width = (float)screenWidth;
    _viewport.Height = (float)screenHeight;
    _viewport.MinDepth = 0.0f;
    _viewport.MaxDepth = 1.0f;
    _viewport.TopLeftX = 0.0f;
    _viewport.TopLeftY = 0.0f;

	// Create the viewport.
    _deviceContext->RSSetViewports(1, &_viewport);

	// Setup the projection matrix.
	float fov = 3.141592654f / 4.0f;
	float cameraNear = 0.03f;
	float cameraFar = 1000.0f;
	float screenAspect = (float)screenWidth / (float)screenHeight;

	// Create the projection matrix for 3D rendering.
	// _projectionMatrix = XMMatrixPerspectiveFovLH(fov, screenAspect, cameraNear, cameraFar);

    // Initialize the world matrix to the identity matrix.
	// _worldMatrix = XMMatrixIdentity();

	// Create an orthographic projection matrix for 2D rendering.
	// _orthoMatrix = XMMatrixOrthographicLH((float)screenWidth, (float)screenHeight, cameraNear, cameraFar);

	// Clear the second depth stencil state before setting the parameters.
	D3D11_DEPTH_STENCIL_DESC depthDisabledStencilDesc;
	ZeroMemory(&depthDisabledStencilDesc, sizeof(depthDisabledStencilDesc));

	// Now create a second depth stencil state which turns off the Z buffer for 2D rendering.  The only difference is
	// that DepthEnable is set to false, all other parameters are the same as the other depth stencil state.
	depthDisabledStencilDesc.DepthEnable = false;
	depthDisabledStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthDisabledStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
	depthDisabledStencilDesc.StencilEnable = true;
	depthDisabledStencilDesc.StencilReadMask = 0xFF;
	depthDisabledStencilDesc.StencilWriteMask = 0xFF;
	depthDisabledStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthDisabledStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	depthDisabledStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthDisabledStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Create the state using the device.
	result = _device->CreateDepthStencilState(&depthDisabledStencilDesc, &_depthDisabledStencilState);
	if(FAILED(result))
	{
		return false;
	}

	// Clear the blend state description.
	D3D11_BLEND_DESC blendStateDescription;
	ZeroMemory(&blendStateDescription, sizeof(D3D11_BLEND_DESC));

	// Create an alpha enabled blend state description.
	blendStateDescription.RenderTarget[0].BlendEnable = TRUE;
	blendStateDescription.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	blendStateDescription.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendStateDescription.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendStateDescription.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendStateDescription.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendStateDescription.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendStateDescription.RenderTarget[0].RenderTargetWriteMask = 0x0f;

	// Create the blend state using the description.
	result = _device->CreateBlendState(&blendStateDescription, &_alphaEnableBlendingState);
	if(FAILED(result))
	{
		return false;
	}

	// Modify the description to create an alpha disabled blend state description.
	blendStateDescription.RenderTarget[0].BlendEnable = FALSE;

	// Create the blend state using the description.
	result = _device->CreateBlendState(&blendStateDescription, &_alphaDisableBlendingState);
	if(FAILED(result))
	{
		return false;
	}

	// ToDo
	_errorShader = new Shader{L"Assets/Shaders/Error.hlsl"};
	_errorShader->Compile(_device);
	_errorMaterial = new Material{_device, _errorShader};

	// ToDo
	size_t globalPropsBufferSize = sizeof(GlobalProperties);
	if (globalPropsBufferSize % 16 != 0)
	{
		globalPropsBufferSize += 16 - globalPropsBufferSize % 16;
	}
	_globalPropertiesBuffer = new ConstBufferData{_device, 0, ShaderUtils::PropertyToID("GlobalProperties"), globalPropsBufferSize, _globalProperties};

	// ToDo
	size_t drawPropsBufferSize = sizeof(DrawProperties);
	if (drawPropsBufferSize % 16 != 0)
	{
		drawPropsBufferSize += 16 - drawPropsBufferSize % 16;
	}
	_drawPropertiesBuffer = new ConstBufferData{_device, 0, ShaderUtils::PropertyToID("DrawProperties"), drawPropsBufferSize, _drawProperties};

    return true;
}

void RenderServer::Shutdown()
{
	// Before shutting down set to windowed mode or when you release the swap chain it will throw an exception.
	if(_swapChain)
	{
		_swapChain->SetFullscreenState(false, NULL);
	}

	if(_alphaEnableBlendingState)
	{
		_alphaEnableBlendingState->Release();
		_alphaEnableBlendingState = 0;
	}

	if(_alphaDisableBlendingState)
	{
		_alphaDisableBlendingState->Release();
		_alphaDisableBlendingState = 0;
	}

	if(_depthDisabledStencilState)
	{
		_depthDisabledStencilState->Release();
		_depthDisabledStencilState = 0;
	}

	if(_rasterState)
	{
		_rasterState->Release();
		_rasterState = 0;
	}

	if(_depthStencilView)
	{
		_depthStencilView->Release();
		_depthStencilView = 0;
	}

	if(_depthStencilState)
	{
		_depthStencilState->Release();
		_depthStencilState = 0;
	}

	if(_depthStencilBuffer)
	{
		_depthStencilBuffer->Release();
		_depthStencilBuffer = 0;
	}

	if(_renderTargetView)
	{
		_renderTargetView->Release();
		_renderTargetView = 0;
	}

	if(_deviceContext)
	{
		_deviceContext->Release();
		_deviceContext = 0;
	}

	if(_device)
	{
		_device->Release();
		_device = 0;
	}

	if(_swapChain)
	{
		_swapChain->Release();
		_swapChain = 0;
	}
}

void RenderServer::PipelineMarkGlobalPropertiesDirty()
{
	_globalPropertiesBuffer->MarkDirty();
}

void RenderServer::PipelineSetCamera(Camera *camera)
{
	_drawProperties->ProjectionMatrix = camera->GetProjectionMatrix();
	_drawPropertiesBuffer->MarkDirty();
}

void RenderServer::PipelineSetModelMatrix(const DirectX::XMMATRIX& matrix)
{
	_drawProperties->ModelMatrix = matrix;
	_drawPropertiesBuffer->MarkDirty();
}

// void RenderServer::PipelineSetMainCameraReference(Camera *camera)
// {
// 	_mainCamera = camera;
// }

void RenderServer::PipelineSetMesh(Mesh *mesh)
{
	constexpr unsigned int offset = 0;
	constexpr unsigned int stride = sizeof(VertexData);

	_deviceContext->IASetVertexBuffers(0, 1, &mesh->vertexBuffer, &stride, &offset);
	_deviceContext->IASetIndexBuffer(mesh->indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void RenderServer::PipelineSetMaterial(Material *material)
{
	// Upload props to GPU buffer if material or shaders were changed
	material->UploadAllProperties(_deviceContext, _globalPropertiesBuffer, _drawPropertiesBuffer);

	_deviceContext->IASetInputLayout(
		material->GetShader()->GetVertexShader()->GetInputLayout());
	_deviceContext->VSSetShader(
		material->GetShader()->GetVertexShader()->AsID3D11(), nullptr, 0);
	_deviceContext->PSSetShader(
		material->GetShader()->GetPixelShader()->AsID3D11(), nullptr, 0);

	static ID3D11Buffer* vsBuffers[3];
	static ID3D11Buffer* psBuffers[3];
	int vsBuffersLength = 0;
	int psBuffersLength = 0;

	material->EnumerateBuffers(vsBuffers, vsBuffersLength, psBuffers, psBuffersLength,
		_globalPropertiesBuffer, _drawPropertiesBuffer);

	_deviceContext->VSSetConstantBuffers(
		0, vsBuffersLength, vsBuffers);
	_deviceContext->PSSetConstantBuffers(
		0, psBuffersLength, psBuffers);
}

void RenderServer::PipelineDrawIndexed(Mesh *mesh)
{
	_deviceContext->DrawIndexed(mesh->indexCount, 0, 0);
}

void RenderServer::BeginScene(DirectX::XMFLOAT4 color)
{
	_deviceContext->ClearRenderTargetView(_renderTargetView, reinterpret_cast<const float*>(&color));
	_deviceContext->ClearDepthStencilView(_depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void RenderServer::EndScene()
{
	if(_isVSyncEnabled)
	{
		// Lock to screen refresh rate.
		_swapChain->Present(1, 0);
	}
	else
	{
		// Present as fast as possible.
		_swapChain->Present(0, 0);
	}
}

void RenderServer::DrawMesh(Mesh *mesh, Material* material, const DirectX::XMMATRIX& matrix, Camera *camera)
{
	PipelineSetModelMatrix(matrix);
	PipelineSetCamera(camera);
	PipelineSetMesh(mesh);
	PipelineSetMaterial(material ? material : _errorMaterial);
	PipelineDrawIndexed(mesh);
}