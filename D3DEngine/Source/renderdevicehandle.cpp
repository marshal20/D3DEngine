#include "renderdevicehandle.h"


namespace DeviceHandle
{
	RenderDevice* pRenderDevice = nullptr;

	ID3D11Device* pDevice = nullptr;
	ID3D11DeviceContext* pContext = nullptr;
}