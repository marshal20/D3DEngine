#include "renderDevice.h"
#include "checks.h"
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <d3d11.h>
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")

struct RenderDevice::D3D11Impl
{
	ID3D11Device* pDevice;
	ID3D11DeviceContext* pContext;
	IDXGISwapChain* pSwapchain;
	ID3D11RenderTargetView* pRenderTargetView;
};

RenderDevice::RenderDevice()
{
	m_impl = new D3D11Impl;
	m_impl->pDevice = nullptr;
	m_impl->pContext = nullptr;
	m_impl->pSwapchain = nullptr;
	m_impl->pRenderTargetView = nullptr;
}

RenderDevice::~RenderDevice()
{
	delete m_impl;
	m_impl = nullptr;
}

void RenderDevice::init(const OutputMode& outputmode, const Window& outputWindow)
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
	ID3D11Texture2D* pBackBuffer;
	bool isWindowed;

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
	m_impl->pDevice->CreateRenderTargetView(pBackBuffer, NULL, &m_impl->pRenderTargetView);

	pBackBuffer->Release();
}

void RenderDevice::cleanup()
{
	setFullscreenState(false);

	if (m_impl->pRenderTargetView)
	{
		m_impl->pRenderTargetView->Release();
		m_impl->pRenderTargetView = nullptr;
	}

	if (m_impl->pSwapchain)
	{
		m_impl->pSwapchain->Release();
		m_impl->pSwapchain = nullptr;
	}

	if (m_impl->pContext)
	{
		m_impl->pContext->Release();
		m_impl->pContext = nullptr;
	}

	if (m_impl->pDevice)
	{
		m_impl->pDevice->Release();
		m_impl->pDevice = nullptr;
	}
}

void RenderDevice::beginScene(float r, float g, float b, float a)
{
	float color[4] = {r, g, b, a};

	m_impl->pContext->ClearRenderTargetView(m_impl->pRenderTargetView, color);

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
	IDXGIFactory* pFactory = nullptr;
	IDXGIAdapter* pAdapter = nullptr;
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

	pAdapter->Release();
	pAdapter = nullptr;
	pFactory->Release();
	pFactory = nullptr;

	return info;
}

std::vector<OutputMode> RenderDevice::getOutputModes()
{
	std::vector<OutputMode> outputModes;
	IDXGIFactory* pFactory = nullptr;
	IDXGIAdapter* pAdapter = nullptr;
	IDXGIOutput* pOutput = nullptr;
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
	pOutput->Release();
	pOutput = nullptr;
	pAdapter->Release();
	pAdapter = nullptr;
	pFactory->Release();
	pFactory = nullptr;

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
