#include "internalstate.h"

namespace ce
{
	namespace state
	{
		ce::RenderContext* m_render_context = nullptr;
		ce::Window* m_window= nullptr;

		namespace d3d
		{
			ID3D11Device* m_device = nullptr;
			ID3D11DeviceContext* m_context = nullptr;
		}
	}
}