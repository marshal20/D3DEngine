#include "device.h"
#include "checks.h"

Device::Device()
{

}

Device::~Device()
{

}

void Device::init()
{
	IDXGIFactory* factory;
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
	float fieldOfView, screenAspect;



	//D3D11CALL(D3D11CreateDeviceAndSwapChain())
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