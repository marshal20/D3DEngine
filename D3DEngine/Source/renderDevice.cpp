#include "renderdevice.h"
#include "checks.h"
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <d3d11.h>
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#include "renderdeviceImpl.h"

struct RenderDevice::RenderBuffers
{
	InterPtr<ID3D11RenderTargetView> pRenderTargetView;
	InterPtr<ID3D11RasterizerState> pRasterState;
	InterPtr<ID3D11Texture2D> pDepthStencilBuffer;
	InterPtr<ID3D11DepthStencilState> pDepthStencilState;
	InterPtr<ID3D11DepthStencilView> pDepthStencilView;
};

RenderDevice::RenderDevice()
{

}

RenderDevice::~RenderDevice()
{

}

void RenderDevice::init(const OutputMode& outputmode, Window& outputWindow)
{
	m_outputmode = outputmode;

	/*IDXGIFactory* factory;
	IDXGIAdapter* adapter;
	IDXGIOutput* adapterOutput;
	unsigned int numModes, i, numerator, denominator;
	unsigned long long stringLength;
	DXGI_MODE_DESC* displayModeList;
	DXGI_ADAPTER_DESC adapterDesc;
	int error;
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	D3D_FEATURE_LEVEL featureLevel;
	ID3D11Texture2D* backBufferPtr;
	D3D11_TEXTURE2D_DESC depthBufferDesc;
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	D3D11_RASTERIZER_DESC rasterDesc;
	D3D11_VIEWPORT viewport;
	float fieldOfView, screenAspect;*/

	D3D_FEATURE_LEVEL featureLevel;
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	InterPtr<ID3D11Texture2D> pBackBuffer;
	D3D11_RASTERIZER_DESC rasterDesc;
	bool isWindowed;

	m_impl->pDevice = nullptr;
	m_impl->pContext = nullptr;
	m_impl->pSwapchain = nullptr;
	m_buffers->pRenderTargetView = nullptr;

	isWindowed = !outputWindow.m_options.fullscreen;
	m_fullscreen = !isWindowed;

	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));
	// setting up the swap chain description.
	swapChainDesc.BufferCount = 1;
	swapChainDesc.BufferDesc.Width = m_outputmode.width;
	swapChainDesc.BufferDesc.Height = m_outputmode.height;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	if (m_vsync_enabled)
	{
		swapChainDesc.BufferDesc.RefreshRate.Numerator = m_outputmode.RefreshRate.Numerator;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = m_outputmode.RefreshRate.Denominator;
	}
	else
	{
		swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	}
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow = (HWND)outputWindow.getNativeHandle();
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.Windowed = isWindowed;

	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapChainDesc.Flags = 0;

	featureLevel = D3D_FEATURE_LEVEL_11_0;

	D3D11CALL(D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, NULL,
		&featureLevel, 1, D3D11_SDK_VERSION, &swapChainDesc,
		&m_impl->pSwapchain, &m_impl->pDevice, NULL, &m_impl->pContext));
	
	// render target view
	m_impl->pSwapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
	m_impl->pDevice->CreateRenderTargetView(pBackBuffer, NULL, &m_buffers->pRenderTargetView);


	// RETHINK // THIS MAKES IT RUN
	D3D11_TEXTURE2D_DESC depthBufferDesc;
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;

	D3D11CALL(m_impl->pDevice->CreateRenderTargetView(pBackBuffer, NULL, &m_buffers->pRenderTargetView));

	pBackBuffer->Release();
	pBackBuffer = 0;

	// Initialize the description of the depth buffer.
	ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));

	// Set up the description of the depth buffer.
	depthBufferDesc.Width = m_outputmode.width;
	depthBufferDesc.Height = m_outputmode.height;
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
	D3D11CALL(m_impl->pDevice->CreateTexture2D(&depthBufferDesc, NULL, &m_buffers->pDepthStencilBuffer));


	// Initialize the description of the stencil state.
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
	D3D11CALL(m_impl->pDevice->CreateDepthStencilState(&depthStencilDesc, &m_buffers->pDepthStencilState));

	// Set the depth stencil state.
	m_impl->pContext->OMSetDepthStencilState(m_buffers->pDepthStencilState, 1);

	// Initialize the depth stencil view.
	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

	// Set up the depth stencil view description.
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	// Create the depth stencil view.
	D3D11CALL(m_impl->pDevice->CreateDepthStencilView(m_buffers->pDepthStencilBuffer, &depthStencilViewDesc, &m_buffers->pDepthStencilView));

	// Bind the render target view and depth stencil buffer to the output render pipeline.
	m_impl->pContext->OMSetRenderTargets(1, &m_buffers->pRenderTargetView, m_buffers->pDepthStencilView);
	// END RETHINK*/


	// resterizing options
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

	D3D11CALL(m_impl->pDevice->CreateRasterizerState(&rasterDesc, &m_buffers->pRasterState));
	m_impl->pContext->RSSetState(m_buffers->pRasterState);

	setViewport(m_outputmode.width, m_outputmode.height);
}

void RenderDevice::cleanup()
{
	setFullscreenState(false);
}

void RenderDevice::beginScene(float r, float g, float b, float a)
{
	float color[4] = {r, g, b, a};

	m_impl->pContext->ClearRenderTargetView(m_buffers->pRenderTargetView, color);

	m_impl->pContext->ClearDepthStencilView(m_buffers->pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

	return;
}

void RenderDevice::endScene()
{
	if (m_vsync_enabled)
	{
		m_impl->pSwapchain->Present(1, 0);
	}
	else
	{
		m_impl->pSwapchain->Present(0, 0);
	}

	return;
}

void RenderDevice::setViewport(int width, int height)
{
	D3D11_VIEWPORT viewport;

	viewport.Width = (float)width;
	viewport.Height = (float)height;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;

	m_impl->pContext->RSSetViewports(1, &viewport);
}

void RenderDevice::setFullscreenState(bool enabled)
{
	m_fullscreen = enabled;

	if(m_fullscreen)
		m_impl->pSwapchain->SetFullscreenState(true, NULL);
	else
		m_impl->pSwapchain->SetFullscreenState(FALSE, NULL);
}


// STATIC FUNCTIONS

AdapterInfo RenderDevice::getAdapterInfo()
{
	AdapterInfo info;
	InterPtr<IDXGIFactory> pFactory;
	InterPtr<IDXGIAdapter> pAdapter;
	DXGI_ADAPTER_DESC adapterDesc;
	char description[128];

	D3D11CALL(CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&pFactory));

	D3D11CALL(pFactory->EnumAdapters(0, &pAdapter));

	D3D11CALL(pAdapter->GetDesc(&adapterDesc));

	memset(description, 0, sizeof(description));
	wcstombs_s(NULL, description, 128, adapterDesc.Description, 128);

	info.Description = description;
	info.VendorId = adapterDesc.VendorId;
	info.DeviceId = adapterDesc.DeviceId;
	info.SubSysId = adapterDesc.SubSysId;
	info.Revision = adapterDesc.Revision;

	info.DedicatedVideoMemory = adapterDesc.DedicatedVideoMemory;
	info.DedicatedSystemMemory = adapterDesc.DedicatedSystemMemory;
	info.SharedSystemMemory = adapterDesc.SharedSystemMemory;

	return info;
}

std::vector<OutputMode> RenderDevice::getOutputModes()
{
	std::vector<OutputMode> outputModes;
	InterPtr<IDXGIFactory> pFactory;
	InterPtr<IDXGIAdapter> pAdapter;
	InterPtr<IDXGIOutput> pOutput;

	DXGI_MODE_DESC* pModeList = nullptr;
	unsigned int numModes;

	D3D11CALL(CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&pFactory));
	D3D11CALL(pFactory->EnumAdapters(0, &pAdapter));
	D3D11CALL(pAdapter->EnumOutputs(0, &pOutput));

	D3D11CALL(pOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, NULL));

	pModeList = new DXGI_MODE_DESC[numModes];
	D3D11CALL(pOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, pModeList));

	outputModes.reserve(numModes);
	for (unsigned int i = 0; i < numModes; i++)
	{
		outputModes.push_back({ pModeList[i].Width, pModeList[i].Height, 
					{pModeList[i].RefreshRate.Numerator, pModeList[i].RefreshRate.Denominator} });
	}

	delete[] pModeList;

	return outputModes;
}

OutputMode RenderDevice::matchOutputMode(unsigned int width, unsigned int height)
{
	std::vector<OutputMode> modes = getOutputModes();
	for(auto& mode: modes)
	{ 
		if (mode.width == width && mode.height == height)
			return mode;
	}
	return { (unsigned int)width, (unsigned int)height, {0, 1} };
}

RenderDeviceImpl* RenderDevice::getImplementation()
{
	return m_impl;
}