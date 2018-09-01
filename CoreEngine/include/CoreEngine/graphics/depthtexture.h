#pragma once

#include <CoreEngine/math/vec2.h>

struct ID3D11Texture2D;
struct ID3D11DepthStencilView;

namespace ce
{
	class DepthTexture
	{
	private:
		ID3D11Texture2D* m_texture = nullptr;
		ID3D11DepthStencilView* m_view = nullptr;

	public:
		DepthTexture();
		~DepthTexture();

		void create(const math::Vec2<int>& size);
		void cleanup();

	private:
		friend class RenderContext;
		ID3D11DepthStencilView* get_view() const;

	};
}
