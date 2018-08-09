#include "renderdevice.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <d3d11.h>
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")

#include "checks.h"
#include "renderdevicehandle.h"
#include "pointerutil.h"

struct RenderDevice::RenderDeviceImpl
{
	InterPtr<ID3D11Device> pDevice;
	InterPtr<ID3D11DeviceContext> pContext;
	InterPtr<IDXGISwapChain> pSwapchain;
};

struct RenderDevice::RenderDeviceBuffers
{
	InterPtr<ID3D11RenderTargetView> pRenderTargetView;
	InterPtr<ID3D11RasterizerState> pRasterState;
	InterPtr<ID3D11Texture2D> pDepthStencilBuffer;
	InterPtr<ID3D11DepthStencilState> pDepthStencilState;
	InterPtr<ID3D11DepthStencilView> pDepthStencilView;
};

RenderDevice::RenderDevice()
{
	m_impl = std::make_shared<RenderDeviceImpl>();
	m_buffers = std::make_unique<RenderDeviceBuffers>();
}

RenderDevice::~RenderDevice()
{

}

void RenderDevice::init(const OutputMode& outputmode, Window& outputWindow, int MSAA_count)
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

	InterPtr<IDXGIFactory> pFactory;
	D3D_FEATURE_LEVEL featureLevel;
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	InterPtr<ID3D11Texture2D> pBackBuffer;
	bool isWindowed;
	unsigned int MSAA_quality;
	HRESULT hr;

	m_impl->pDevice = nullptr;
	m_impl->pContext = nullptr;
	m_impl->pSwapchain = nullptr;
	m_buffers->pRenderTargetView = nullptr;

	isWindowed = !outputWindow.m_options.fullscreen;
	m_fullscreen = !isWindowed;

	hr = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&pFactory);
	checks::D3D11CALL_ERR(hr, "CreateDXGIFactory(...) failed.");

	featureLevel = D3D_FEATURE_LEVEL_11_0;
	hr = D3D11CreateDevice(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, NULL,
		&featureLevel, 1, D3D11_SDK_VERSION, &m_impl->pDevice, NULL, &m_impl->pContext);
	checks::D3D11CALL_ERR(hr, "D3D11CreateDevice(...) failed.");

	hr = m_impl->pDevice->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, MSAA_count, &MSAA_quality);
	checks::D3D11CALL_WRN(hr, "ID3D11Device::CheckMultisampleQualityLevels(...) failed.");
	MSAA_quality = (MSAA_quality > 0) ? MSAA_quality - 1 : 0;

	// setting up the swap chain description.
	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));
	swapChainDesc.BufferCount = 1;
	swapChainDesc.BufferDesc.Width = m_outputmode.width;
	swapChainDesc.BufferDesc.Height = m_outputmode.height;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	if (m_vsync_enabled) {
		swapChainDesc.BufferDesc.RefreshRate.Numerator = m_outputmode.RefreshRate.Numerator;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = m_outputmode.RefreshRate.Denominator;
	}
	else {
		swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
		swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	}
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow = (HWND)outputWindow.getNativeHandle();
	swapChainDesc.SampleDesc.Count = MSAA_count;
	swapChainDesc.SampleDesc.Quality = MSAA_quality;
	swapChainDesc.Windowed = isWindowed;

	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapChainDesc.Flags = 0;

	hr = pFactory->CreateSwapChain(m_impl->pDevice, &swapChainDesc, &m_impl->pSwapchain);
	checks::D3D11CALL_ERR(hr, "IDXGIFactory::CreateSwapChain(...) failed.");

	// render target view
	m_impl->pSwapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);

	hr = m_impl->pDevice->CreateRenderTargetView(pBackBuffer, NULL, &m_buffers->pRenderTargetView);
	checks::D3D11CALL_ERR(hr, "ID3D11Device::CreateRenderTargetView(...) failed.");


	// RETHINK // THIS MAKES IT RUN
	D3D11_TEXTURE2D_DESC depthBufferDesc;
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;

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
	depthBufferDesc.SampleDesc.Count = MSAA_count;
	depthBufferDesc.SampleDesc.Quality = MSAA_quality;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;

	// Create the texture for the depth buffer using the filled out description.
	hr = m_impl->pDevice->CreateTexture2D(&depthBufferDesc, NULL, &m_buffers->pDepthStencilBuffer);
	checks::D3D11CALL_ERR(hr, "ID3D11Device::CreateTexture2D(...) failed.");

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
	hr = m_impl->pDevice->CreateDepthStencilState(&depthStencilDesc, &m_buffers->pDepthStencilState);
	checks::D3D11CALL_ERR(hr, "ID3D11Device::CreateDepthStencilState(...) failed.");

	// Set the depth stencil state.
	m_impl->pContext->OMSetDepthStencilState(m_buffers->pDepthStencilState, 1);

	// Initialize the depth stencil view.
	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

	// Set up the depth stencil view description.
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	// Create the depth stencil view.
	hr = m_impl->pDevice->CreateDepthStencilView(m_buffers->pDepthStencilBuffer, &depthStencilViewDesc, &m_buffers->pDepthStencilView);
	checks::D3D11CALL_ERR(hr, "ID3D11Device::CreateDepthStencilView(...) failed.");

	// Bind the render target view and depth stencil buffer to the output render pipeline.
	m_impl->pContext->OMSetRenderTargets(1, &m_buffers->pRenderTargetView, m_buffers->pDepthStencilView);
	// END RETHINK*/

	setRestrizerOptions({RestrizerOptions::CullMode::Back, 
						RestrizerOptions::FillMode::Solid, false, true, false});

	setViewport(m_outputmode.width, m_outputmode.height);
	
	DeviceHandle::pRenderDevice = this;
	DeviceHandle::pDevice = m_impl->pDevice;
	DeviceHandle::pContext = m_impl->pContext;
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

void RenderDevice::setRestrizerOptions(const RestrizerOptions& resOpt)
{
	D3D11_RASTERIZER_DESC rasterDesc;
	D3D11_CULL_MODE cullmode;
	D3D11_FILL_MODE fillmode;
	HRESULT hr;

	cullmode = D3D11_CULL_BACK;
	switch (resOpt.cullmode)
	{
	case RestrizerOptions::CullMode::Back:
		cullmode = D3D11_CULL_BACK;
		break;
	case RestrizerOptions::CullMode::Front:
		cullmode = D3D11_CULL_FRONT;
		break;
	case RestrizerOptions::CullMode::None:
		cullmode = D3D11_CULL_NONE;
		break;
	default:
		break;
	}

	fillmode = D3D11_FILL_SOLID;
	switch (resOpt.fillmode)
	{
	case RestrizerOptions::FillMode::Solid:
		fillmode = D3D11_FILL_SOLID;
		break;
	case RestrizerOptions::FillMode::Wireframe:
		fillmode = D3D11_FILL_WIREFRAME;
		break;
	default:
		break;
	}

	rasterDesc.AntialiasedLineEnable = false;
	rasterDesc.CullMode = cullmode;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = true;
	rasterDesc.FillMode = fillmode;
	rasterDesc.FrontCounterClockwise = resOpt.frontCounterClockwise;
	rasterDesc.MultisampleEnable = resOpt.multisampleEnable;
	rasterDesc.ScissorEnable = resOpt.scissorEnable;
	rasterDesc.SlopeScaledDepthBias = 0.0f;

	if (m_buffers->pRasterState)
	{
		m_buffers->pRasterState->Release();
		m_buffers->pRasterState = nullptr;
	}

	hr = m_impl->pDevice->CreateRasterizerState(&rasterDesc, &m_buffers->pRasterState);
	checks::D3D11CALL_ERR(hr, "ID3D11Device::CreateRasterizerState(...) failed.");

	m_impl->pContext->RSSetState(m_buffers->pRasterState);
}

void RenderDevice::setFullscreenState(bool enabled)
{
	m_fullscreen = enabled;

	if(m_fullscreen)
		m_impl->pSwapchain->SetFullscreenState(true, NULL);
	else
		m_impl->pSwapchain->SetFullscreenState(FALSE, NULL);
}

float RenderDevice::getAspectRatio() const
{
	return (float)m_outputmode.width / (float)m_outputmode.height;
}


// STATIC FUNCTIONS

AdapterInfo RenderDevice::getAdapterInfo()
{
	AdapterInfo info;
	InterPtr<IDXGIFactory> pFactory;
	InterPtr<IDXGIAdapter> pAdapter;
	DXGI_ADAPTER_DESC adapterDesc;
	char description[128];
	HRESULT hr;

	hr = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&pFactory);
	checks::D3D11CALL_WRN(hr, "CreateDXGIFactory(...) failed.");

	hr = pFactory->EnumAdapters(0, &pAdapter);
	checks::D3D11CALL_WRN(hr, "IDXGIFactory::EnumAdapters(...) failed.");

	hr = pAdapter->GetDesc(&adapterDesc);
	checks::D3D11CALL_WRN(hr, "IDXGIAdapter::GetDesc(...) failed.");

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
	HRESULT hr;

	hr = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&pFactory);
	checks::D3D11CALL_WRN(hr, "CreateDXGIFactory(...) failed.");

	hr = pFactory->EnumAdapters(0, &pAdapter);
	checks::D3D11CALL_WRN(hr, "IDXGIFactory::EnumAdapters(...) failed.");

	hr = pAdapter->EnumOutputs(0, &pOutput);
	checks::D3D11CALL_WRN(hr, "IDXGIAdapter::EnumOutputs(...) failed.");

	hr = pOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, NULL);
	checks::D3D11CALL_WRN(hr, "IDXGIOutput::GetDisplayModeList(...) failed.");

	pModeList = new DXGI_MODE_DESC[numModes];

	hr = pOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, pModeList);
	checks::D3D11CALL_WRN(hr, "IDXGIOutput::GetDisplayModeList(...) failed.");

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
