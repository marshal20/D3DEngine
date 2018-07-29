#pragma once
#include <d3d11.h>
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#include <string>

struct AdapterInfo
{
	std::string Description;
	unsigned int VendorId;
	unsigned int DeviceId;
	unsigned int SubSysId;
	unsigned int Revision;
	size_t DedicatedVideoMemory;
	size_t DedicatedSystemMemory;
	size_t SharedSystemMemory;
};

class Device
{
public:
	Device();
	~Device();
	void init();

	static AdapterInfo getAdapterInfo();

private:
	//class DeviceImpl;
	//DeviceImpl* m_impl = nullptr;

	ID3D11Device* m_pDevice;
	ID3D11DeviceContext* m_context;
	IDXGISwapChain* m_swapchain;
};