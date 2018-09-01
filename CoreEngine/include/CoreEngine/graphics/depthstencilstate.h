#pragma once

// TODO: Add more options
struct ID3D11DepthStencilState;

namespace ce
{
	class DepthStencilState
	{
	private:
		ID3D11DepthStencilState* m_depth_stencil_state = nullptr;

	public:
		DepthStencilState();
		~DepthStencilState();

		void init(bool depth_enable = true, bool stencil_enable = true);
		void cleanup();
		void set_main();
	};
}
