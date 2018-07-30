#pragma once
#include <d3d11.h>
#include "pointerutil.h"

struct RenderDeviceImpl
{
	InterPtr<ID3D11Device> pDevice;
	InterPtr<ID3D11DeviceContext> pContext;
	InterPtr<IDXGISwapChain> pSwapchain;
};