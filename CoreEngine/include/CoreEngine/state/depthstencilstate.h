#pragma once

// TODO: Add more options
struct ID3D11DepthStencilState;

namespace ce
{
	class DepthStencilState
	{
	public:
		DepthStencilState();
		~DepthStencilState();

		void init();
		void cleanup();
		void set_main();

		void set_depth_enable(bool enable);
		void set_stencil_enable(bool enable);

	private:
		ID3D11DepthStencilState* m_depth_stencil_state = nullptr;

		bool m_depth_enable = true;
		bool m_stencil_enable = true;
	};
}
