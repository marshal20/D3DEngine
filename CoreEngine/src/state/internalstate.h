#pragma once

#include <d3d11.h>
#include <CoreEngine\rendercontext.h>
#include <CoreEngine\window.h>

namespace ce
{
	namespace state
	{
		extern ce::RenderContext* m_render_context;
		extern ce::Window* m_window;

		namespace d3d
		{
			extern ID3D11Device* m_device;
			extern ID3D11DeviceContext* m_context;
		}
	}
}
