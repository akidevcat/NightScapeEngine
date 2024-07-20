#include "RenderServer.h"

#include <iostream>

#include "../data/LightsProperties.h"
#include "../render/GlobalProperties.h"

NSE::RenderServer::RenderServer()
{
	_isVSyncEnabled = true;
}

NSE::RenderServer::~RenderServer()
{

}

bool NSE::RenderServer::Initialize(int screenWidth, int screenHeight, HWND hwnd)
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

    unsigned int refreshRateNum = 0;
    unsigned int refreshRateDenum = 0;

    // Now go through all the display modes and find the one that matches the screen width and height.
    // When a match is found store the numerator and denominator of the refresh rate for that monitor.
    for (unsigned i = 0; i < numModes; i++)
    {
        if(displayModeList[i].Width != (unsigned int)screenWidth || displayModeList[i].Height != (unsigned int)screenHeight)
            continue;

		auto num = displayModeList[i].RefreshRate.Numerator;
		auto denom = displayModeList[i].RefreshRate.Denominator;

    	// ToDo
    	if (num > 1000)
    	{
    		continue;
    	}

    	refreshRateNum = num;
    	refreshRateDenum = denom;
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
    swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;

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
	// ToDo
	// swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;

	// Don't set the advanced flags.
	swapChainDesc.Flags = 0;

	// Set the feature level to DirectX 11.
	D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_0;

	UINT creationFlags = 0;
#if defined(_DEBUG)
	creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	// Create the swap chain, Direct3D device, and Direct3D device context.
	result = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, creationFlags, &featureLevel, 1,
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

	result = _device->CreateDepthStencilState(&depthStencilDesc, &_depthReadWriteStencilState);
	if(FAILED(result))
	{
		return false;
	}

	// Set the depth stencil state.
	_deviceContext->OMSetDepthStencilState(_depthReadWriteStencilState, 1);

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

	// Clear the second depth stencil state before setting the parameters.
	D3D11_DEPTH_STENCIL_DESC depthReadOnlyStencilDesc;
	ZeroMemory(&depthReadOnlyStencilDesc, sizeof(depthReadOnlyStencilDesc));

	depthReadOnlyStencilDesc.DepthEnable = true;
	depthReadOnlyStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	depthReadOnlyStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
	depthReadOnlyStencilDesc.StencilEnable = true;
	depthReadOnlyStencilDesc.StencilReadMask = 0xFF;
	depthReadOnlyStencilDesc.StencilWriteMask = 0x00;
	depthReadOnlyStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthReadOnlyStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthReadOnlyStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthReadOnlyStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	depthReadOnlyStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthReadOnlyStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthReadOnlyStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthReadOnlyStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Create the state using the device.
	result = _device->CreateDepthStencilState(&depthReadOnlyStencilDesc, &_depthReadStencilState);
	if(FAILED(result))
	{
		return false;
	}

	D3D11_DEPTH_STENCIL_DESC depthDisabledStencilDesc;
	ZeroMemory(&depthDisabledStencilDesc, sizeof(depthDisabledStencilDesc));

	depthDisabledStencilDesc.DepthEnable = false;
	depthDisabledStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	depthDisabledStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
	depthDisabledStencilDesc.StencilEnable = false;
	depthDisabledStencilDesc.StencilReadMask = 0x00;
	depthDisabledStencilDesc.StencilWriteMask = 0x00;
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

	_globalShaderInputs = new ShaderInputsData{};

	_globalPropertiesBuffer = CreateObject<GraphicsBuffer>(GraphicsBuffer::Target::Constant, sizeof(GlobalProperties), true);
	_drawPropertiesBuffer = CreateObject<GraphicsBuffer>(GraphicsBuffer::Target::Constant, sizeof(DrawProperties), true);
	_lightsPropertiesBuffer = CreateObject<GraphicsBuffer>(GraphicsBuffer::Target::Constant, sizeof(LightsProperties), true);

	D3D11_SAMPLER_DESC samplerDesc;

	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	result = _device->CreateSamplerState(&samplerDesc, &_defaultPointSampler);
	if (FAILED(result))
	{
		return false;
	}

	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	result = _device->CreateSamplerState(&samplerDesc, &_defaultLinearSampler);
	if (FAILED(result))
	{
		return false;
	}

	// Create Resources ToDo
	_globalShaderInputs->SetSampler(ShaderUtils::PropertyToID("_LinearSampler"), _defaultLinearSampler);
	_globalShaderInputs->SetSampler(ShaderUtils::PropertyToID("_PointSampler"), _defaultPointSampler);

	D3D11_BLEND_DESC blendDescription;
	ZeroMemory(&blendDescription, sizeof(D3D11_BLEND_DESC));



	blendDescription.RenderTarget[0].BlendEnable = TRUE;
	blendDescription.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	blendDescription.RenderTarget[0].DestBlend = D3D11_BLEND_ZERO;
	blendDescription.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendDescription.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendDescription.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendDescription.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendDescription.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	_defaultOpaqueBlendState = CreateObject<BlendState>(blendDescription);

	blendDescription.RenderTarget[0].BlendEnable = TRUE;
	blendDescription.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendDescription.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
	blendDescription.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendDescription.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ZERO;
	blendDescription.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
	blendDescription.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendDescription.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	_defaultAdditiveBlendState = CreateObject<BlendState>(blendDescription);

	blendDescription.RenderTarget[0].BlendEnable = TRUE;
	blendDescription.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendDescription.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendDescription.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendDescription.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_SRC_ALPHA;
	blendDescription.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;
	blendDescription.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendDescription.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	_defaultTransparentBlendState = CreateObject<BlendState>(blendDescription);

	blendDescription.RenderTarget[0].BlendEnable = TRUE;
	blendDescription.RenderTarget[0].SrcBlend = D3D11_BLEND_INV_DEST_COLOR;
	blendDescription.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_COLOR;
	blendDescription.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendDescription.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ZERO;
	blendDescription.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
	blendDescription.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendDescription.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	_defaultInvertBlendState = CreateObject<BlendState>(blendDescription);

	// ToDo
	_errorShader = CreateObject<Shader>(L"Assets/Shaders/Error.hlsl");
	_errorShader->Compile();
	_errorMaterial = CreateObject<Material>(_errorShader);

	// ToDo
	_primitiveQuadMesh = CreateObject<Mesh>(4, 2 * 3);
	auto pqmVertices = _primitiveQuadMesh->GetVertices();
	auto pqmIndices = _primitiveQuadMesh->GetIndices();
	pqmVertices[0] = VertexData{{-1, -1, 0}, {0, 0, -1}, {0, 0}};
	pqmVertices[1] = VertexData{{-1, 1, 0}, {0, 0, -1}, {0, 1}};
	pqmVertices[2] = VertexData{{1, 1, 0}, {0, 0, -1}, {1, 1}};
	pqmVertices[3] = VertexData{{1, -1, 0}, {0, 0, -1}, {1, 0}};

	pqmIndices[0] = 0;
	pqmIndices[1] = 1;
	pqmIndices[2] = 2;
	pqmIndices[3] = 2;
	pqmIndices[4] = 3;
	pqmIndices[5] = 0;

	_primitiveQuadMesh->Upload();

    return true;
}

void NSE::RenderServer::Shutdown()
{
	// Before shutting down set to windowed mode or when you release the swap chain it will throw an exception.
	if(_swapChain)
	{
		_swapChain->SetFullscreenState(false, nullptr);
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

	if(_depthReadStencilState)
	{
		_depthReadStencilState->Release();
		_depthReadStencilState = 0;
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

	if(_depthReadWriteStencilState)
	{
		_depthReadWriteStencilState->Release();
		_depthReadWriteStencilState = 0;
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

	if (_defaultPointSampler)
	{
		_defaultPointSampler->Release();
		_defaultPointSampler = nullptr;
	}

	if (_defaultLinearSampler)
	{
		_defaultLinearSampler->Release();
		_defaultLinearSampler = nullptr;
	}

	DestroyObject(_globalPropertiesBuffer);
	DestroyObject(_drawPropertiesBuffer);
}

void NSE::RenderServer::PipelineSetCamera(const NSE_Camera& camera)
{
	auto drawProperties = _drawPropertiesBuffer->As<DrawProperties>();

	drawProperties->ProjectionMatrix = camera->GetProjectionMatrix();
	drawProperties->ViewMatrix = camera->GetViewMatrix();
}

void NSE::RenderServer::PipelineSetModelMatrix(const DirectX::XMMATRIX& matrix)
{
	auto drawProperties = _drawPropertiesBuffer->As<DrawProperties>();

	drawProperties->ModelMatrix = matrix;
}

void NSE::RenderServer::PipelineSetMesh(const NSE_Mesh& mesh)
{
	constexpr unsigned int offset = 0;
	constexpr unsigned int stride = sizeof(VertexData);

	// ToDo invalidate mesh on upload
	if (!_currentStateMesh || _currentStateMesh.get() != mesh.get())
	{
		auto vertexBuffers = mesh->GetVertexBuffer()->GetD3DBuffer();

		_deviceContext->IASetVertexBuffers(0, 1, &vertexBuffers, &stride, &offset);
		_deviceContext->IASetIndexBuffer(mesh->GetIndexBuffer()->GetD3DBuffer(), DXGI_FORMAT_R32_UINT, 0);
		_currentStateMesh = mesh;
	}

	if (_currentStatePrimitiveTopology != mesh->GetTopology())
	{
		_deviceContext->IASetPrimitiveTopology(mesh->GetTopology());
		_currentStatePrimitiveTopology = mesh->GetTopology();
	}
}

void NSE::RenderServer::PipelineSetMaterial(const NSE_Material& material)
{
	// Upload props to GPU buffer if material or shaders were changed
	material->Upload();

	if (_currentStateBlend != material->GetBlendState())
	{
		_deviceContext->OMSetBlendState((ID3D11BlendState*)*material->GetBlendState(), nullptr, 0xffffffff);
		_currentStateBlend = material->GetBlendState();
	}

	if (_currentStateDepthWrite != material->GetDepthWrite())
	{
		switch (material->GetDepthWrite())
		{
			case ShaderDepthState::Disabled:
				_deviceContext->OMSetDepthStencilState(_depthDisabledStencilState, 1);
				break;
			case ShaderDepthState::OnlyRead:
				_deviceContext->OMSetDepthStencilState(_depthReadStencilState, 1);
				break;
			case ShaderDepthState::ReadWrite:
				_deviceContext->OMSetDepthStencilState(_depthReadWriteStencilState, 1);
				break;
		}

		// if (_currentStateDepthWrite)
		// {
		// 	_deviceContext->OMSetDepthStencilState(_depthReadStencilState, 1);
		// }
		// else
		// {
		// 	_deviceContext->OMSetDepthStencilState(_depthReadWriteStencilState, 1);
		// }
		_currentStateDepthWrite = material->GetDepthWrite();
	}

	// Map Shaders

	if (_currentStateVertexShader != material->GetShader()->GetVertexShader()->AsID3D11())
	{
		_deviceContext->IASetInputLayout(
			material->GetShader()->GetVertexShader()->GetInputLayout());
		_deviceContext->VSSetShader(
			material->GetShader()->GetVertexShader()->AsID3D11(), nullptr, 0);

		_currentStateVertexShader = material->GetShader()->GetVertexShader()->AsID3D11();
	}

	if (_currentStatePixelShader != material->GetShader()->GetPixelShader()->AsID3D11())
	{
		_deviceContext->PSSetShader(
			material->GetShader()->GetPixelShader()->AsID3D11(), nullptr, 0);

		_currentStatePixelShader = material->GetShader()->GetPixelShader()->AsID3D11();

	}

	// Map Constant Buffers

	{
		static ID3D11Buffer* vsBuffers[D3D11_COMMONSHADER_CONSTANT_BUFFER_HW_SLOT_COUNT];
		int vsBuffersLength = 0;

		material->EnumerateVertexConstantBuffers(vsBuffers, vsBuffersLength);

		if (vsBuffersLength > 0)
		{
			_deviceContext->VSSetConstantBuffers(0, vsBuffersLength, vsBuffers);
		}
	}
	{
		static ID3D11Buffer* psBuffers[D3D11_COMMONSHADER_CONSTANT_BUFFER_HW_SLOT_COUNT];
		int psBuffersLength = 0;

		material->EnumeratePixelConstantBuffers(psBuffers, psBuffersLength);

		if (psBuffersLength > 0)
		{
			_deviceContext->PSSetConstantBuffers(0, psBuffersLength, psBuffers);
		}
	}

	// Map Resources / Samplers

	static ID3D11ShaderResourceView* resources[D3D11_COMMONSHADER_INPUT_RESOURCE_REGISTER_COUNT] = {};
	static ID3D11SamplerState*		 samplers[D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT] = {};

	memset(resources, 0, sizeof(resources));
	memset(samplers, 0, sizeof(samplers));

	UINT resourceBindCount = 0;
	UINT samplersBindCount = 0;

	for(auto [nameID, bindDescription] : *material->GetShader()->GetVertexShader()->GetInputsDescription())
	{
		auto type = bindDescription.Type;
		auto bindPoint = bindDescription.BindPoint;
		bool flFound = false;

		switch (type)
		{
			// Resources ToDo
			case D3D_SIT_TEXTURE:
			case D3D_SIT_STRUCTURED:

				ID3D11ShaderResourceView* resource;

				if (!flFound)
				{
					flFound |= material->GetVSInputs()->GetResource(nameID, resource);
				}
				if (!flFound)
				{
					flFound |= _globalShaderInputs->GetResource(nameID, resource);
				}

				if (flFound)
				{
					resources[bindPoint] = resource;
					resourceBindCount = max(resourceBindCount, bindPoint + 1);
				}

			break;

			// Samplers
			case D3D_SIT_SAMPLER:

				ID3D11SamplerState* sampler;

				if (!flFound)
				{
					flFound |= material->GetVSInputs()->GetSampler(nameID, sampler);
				}
				if (!flFound)
				{
					flFound |= _globalShaderInputs->GetSampler(nameID, sampler);
				}

				if (flFound)
				{
					samplers[bindPoint] = sampler;
					samplersBindCount = max(samplersBindCount, bindPoint + 1);
				}

				break;
			default:
				break;
		}
	}

	if (resourceBindCount > 0)
	{
		_deviceContext->VSSetShaderResources(0, resourceBindCount, resources);
		_deviceContext->VSSetSamplers(0, samplersBindCount, samplers);
	}

	memset(resources, 0, sizeof(resources));
	memset(resources, 0, sizeof(samplers));

	resourceBindCount = 0;
	samplersBindCount = 0;

	for(auto [nameID, bindDescription] : *material->GetShader()->GetPixelShader()->GetInputsDescription())
	{
		auto type = bindDescription.Type;
		auto bindPoint = bindDescription.BindPoint;
		bool flFound = false;

		switch (type)
		{
			// Resources ToDo
			case D3D_SIT_TEXTURE:

				ID3D11ShaderResourceView* resource;

			if (!flFound)
			{
				flFound |= material->GetPSInputs()->GetResource(nameID, resource);
			}
			if (!flFound)
			{
				flFound |= _globalShaderInputs->GetResource(nameID, resource);
			}

			if (flFound)
			{
				resources[bindPoint] = resource;
				resourceBindCount = max(resourceBindCount, bindPoint + 1);
			}

			break;

			// Samplers
			case D3D_SIT_SAMPLER:

				ID3D11SamplerState* sampler;

			if (!flFound)
			{
				flFound |= material->GetPSInputs()->GetSampler(nameID, sampler);
			}
			if (!flFound)
			{
				flFound |= _globalShaderInputs->GetSampler(nameID, sampler);
			}

			if (flFound)
			{
				samplers[bindPoint] = sampler;
				samplersBindCount = max(samplersBindCount, bindPoint + 1);
			}

			break;
			default:
				break;
		}
	}

	if (resourceBindCount > 0)
	{
		_deviceContext->PSSetShaderResources(0, resourceBindCount, resources);
		_deviceContext->PSSetSamplers(0, samplersBindCount, samplers);
	}
}

void NSE::RenderServer::PipelineDrawIndexed(const NSE_Mesh& mesh, int indexCount)
{
	_deviceContext->DrawIndexed(indexCount >= 0 ? indexCount : mesh->GetIndexCount(), 0, 0);
}

void NSE::RenderServer::PipelineSetRenderTargets(ID3D11RenderTargetView *colorTarget,
	ID3D11DepthStencilView *depthTarget, const D3D11_VIEWPORT& viewport)
{
	_deviceContext->OMSetRenderTargets(1, &colorTarget, depthTarget);
	_deviceContext->RSSetViewports(1, &viewport);

	auto drawProperties = _drawPropertiesBuffer->As<DrawProperties>();

	drawProperties->TargetResolutionX = (uint32_t)viewport.Width;
	drawProperties->TargetResolutionY = (uint32_t)viewport.Height;
}

void NSE::RenderServer::PipelineSetRenderTargets(const NSE_RenderTexture& colorTarget,
	const NSE_RenderTexture& depthTarget)
{
	ID3D11RenderTargetView* rtv = colorTarget ? colorTarget->RenderTargetView() : _renderTargetView;
	ID3D11DepthStencilView* dsv = depthTarget ? depthTarget->DepthStencilView() : _depthStencilView;
	D3D11_VIEWPORT viewport = colorTarget ? D3D11_VIEWPORT{0, 0, (float)colorTarget->Width(), (float)colorTarget->Height(), 0.0f, 1.0f} : _viewport; // ToDo

	if (rtv != _currentRenderTargetView || dsv != _currentDepthStencilView)
	{
		_deviceContext->OMSetRenderTargets(1, &rtv, dsv);
		_deviceContext->RSSetViewports(1, &viewport);

		_currentRenderTargetView = rtv;
		_currentDepthStencilView = dsv;
	}

	auto drawProperties = _drawPropertiesBuffer->As<DrawProperties>();

	drawProperties->TargetResolutionX = (uint32_t)viewport.Width;
	drawProperties->TargetResolutionY = (uint32_t)viewport.Height;
}

void NSE::RenderServer::ClearRenderTextureColor(const NSE_RenderTexture& target, DirectX::XMFLOAT4 color) const
{
	_deviceContext->ClearRenderTargetView(target->RenderTargetView(), reinterpret_cast<const float*>(&color));
}

void NSE::RenderServer::ClearRenderTextureDepth(const NSE_RenderTexture& target, float depth) const
{
	_deviceContext->ClearDepthStencilView(target->DepthStencilView(), D3D11_CLEAR_DEPTH, depth, 0);
}

void NSE::RenderServer::PipelineClearRenderTarget(bool clearColor, bool clearDepth, DirectX::XMFLOAT4 color,
	float depth)
{
	if (clearColor)
	{
		_deviceContext->ClearRenderTargetView(_renderTargetView, reinterpret_cast<const float*>(&color));
	}

	if (clearDepth)
	{
		_deviceContext->ClearDepthStencilView(_depthStencilView, D3D11_CLEAR_DEPTH, depth, 0);
	}
}

void NSE::RenderServer::Present()
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

void NSE::RenderServer::DrawMesh(const NSE_Mesh& mesh, const NSE_Material& material, const DirectX::XMMATRIX& matrix, const NSE_Camera& camera, size_t objectID, int indexCount)
{
	PipelineSetModelMatrix(matrix);
	PipelineSetCamera(camera);
	PipelineSetRenderTargets(camera->colorTarget, camera->depthTarget);
	PipelineSetMesh(mesh);
	PipelineSetMaterial(material ? material : _errorMaterial);

	auto drawProperties = _drawPropertiesBuffer->As<DrawProperties>();
	drawProperties->ObjectID = objectID;

	_globalPropertiesBuffer->Upload();
	_drawPropertiesBuffer->Upload();
	_lightsPropertiesBuffer->Upload();

	PipelineDrawIndexed(mesh, indexCount);
}
