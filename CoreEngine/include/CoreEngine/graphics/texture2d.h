#pragma once

#include "../math/vec2.h"

struct ID3D11Texture2D;
struct ID3D11ShaderResourceView;
struct ID3D11SamplerState;

namespace ce
{
	class Image;

	class Texture2D
	{
	public:
		struct Sample
		{
			enum class Filter { Point = 1, Bilinear, Trilinear, Anisotropic };
			enum class Wrap { Repeat = 1, Clamp, Mirror, MirrorOnce };
			Filter filter;
			Wrap wrap_u;
			Wrap wrap_v;
		};

	private:
		math::Vec2<int> m_size;
		ID3D11Texture2D* m_texture = nullptr;
		ID3D11ShaderResourceView* m_view = nullptr;
		ID3D11SamplerState* m_sampler_state = nullptr;

	public:
		Texture2D();
		~Texture2D();

		void create(const math::Vec2<int>& size);
		void cleanup();
		void update_from_memory(const void* data);
		void update_from_image(const Image* image);
		void set_sample_options(const Sample& sample_options);

		void use(const unsigned int slot);
	};
}