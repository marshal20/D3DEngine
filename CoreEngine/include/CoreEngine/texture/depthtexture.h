#pragma once

#include <CoreEngine\math\vec.h>

struct ID3D11Texture2D;
struct ID3D11DepthStencilView;

namespace ce
{
	class DepthTexture
	{
	public:
		DepthTexture();
		~DepthTexture();

		void init(const math::Vec2<int>& size);
		void cleanup();

	private:
		friend class RenderContext;
		ID3D11DepthStencilView* get_view() const;

	private:
		ID3D11Texture2D* m_texture = nullptr;
		ID3D11DepthStencilView* m_view = nullptr;
	};
}
