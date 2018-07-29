#include "device.h"
#include "checks.h"
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <d3d11.h>
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")

struct Device::D3D11Impl
{
	ID3D11Device* pDevice;
	ID3D11DeviceContext* pContext;
	IDXGISwapChain* pSwapchain;
};

Device::Device()
{
	m_impl = new D3D11Impl;
	m_impl->pDevice = nullptr;
	m_impl->pContext = nullptr;
	m_impl->pSwapchain = nullptr;
}

Device::~Device()
{
	delete m_impl;
	m_impl = nullptr;
}

void Device::init(const OutputMode& outputmode, const Window& outputWindow)
{


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

	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));
	
	swapChainDesc.BufferCount = 1;
	swapChainDesc.BufferDesc.Width = outputmode.width;
	swapChainDesc.BufferDesc.Height = outputmode.height;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	if (m_vsync_enabled)
	{
		swapChainDesc.BufferDesc.RefreshRate.Numerator = numerator;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = denominator;
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
	// ------------------------- TESTING ------------------------
	swapChainDesc.Windowed = true;

	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapChainDesc.Flags = 0;

	featureLevel = D3D_FEATURE_LEVEL_11_0;

	D3D11CALL(D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, NULL,
		&featureLevel, 1, D3D11_SDK_VERSION, &swapChainDesc,
		&m_impl->pSwapchain, &m_impl->pDevice, NULL, &m_impl->pContext));


}


AdapterInfo Device::getAdapterInfo()
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

std::vector<OutputMode> Device::getOutputModes()
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

OutputMode Device::getOutputMode(int width, int height)
{
	std::vector<OutputMode> modes = getOutputModes();
	for(auto& mode: modes)
	{ 
		if (mode.width == width && mode.height == height)
			return mode;
	}
	return { (unsigned int)width, (unsigned int)height, {0, 1} };
}
