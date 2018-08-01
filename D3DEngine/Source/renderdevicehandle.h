#pragma once
#include "renderdevice.h"

#include <d3d11.h>

namespace DeviceHandle
{
	extern RenderDevice* pRenderDevice;

	extern ID3D11Device* pDevice;
	extern ID3D11DeviceContext* pContext;
}