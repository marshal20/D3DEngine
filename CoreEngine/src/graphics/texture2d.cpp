#include <CoreEngine/graphics/texture2d.h>

#include <d3d11.h>

#include <CoreEngine/graphics/rendercontext.h>
#include <CoreEngine/filesystem/image.h>
#include "../utils/callcheck.h"
#include "../utils/safemem.h"

namespace ce
{
	D3D11_FILTER get_filter(const Texture2D::Sample::Filter filter);
	D3D11_TEXTURE_ADDRESS_MODE get_wrap(const Texture2D::Sample::Wrap wrap);

	Texture2D::Texture2D()
	{
	}

	Texture2D::~Texture2D()
	{
	}

	void Texture2D::create(const math::Vec2<int>& size)
	{
		HRESULT hr;
		D3D11_TEXTURE2D_DESC texture2d_desc;
		D3D11_SHADER_RESOURCE_VIEW_DESC resource_view_desc;

		m_size = size;

		ZeroMemory(&texture2d_desc, sizeof(texture2d_desc));
		texture2d_desc.Width = size.x;
		texture2d_desc.Height = size.y;
		texture2d_desc.MipLevels = 0;
		texture2d_desc.ArraySize = 1;
		texture2d_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		texture2d_desc.SampleDesc.Count = 1;
		texture2d_desc.SampleDesc.Quality = 0;
		texture2d_desc.Usage = D3D11_USAGE_DEFAULT;
		texture2d_desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
		texture2d_desc.CPUAccessFlags = 0;
		texture2d_desc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;
		hr = RenderContext::get_device()->CreateTexture2D(&texture2d_desc, NULL, &m_texture);
		CHECK_HR(hr);

		ZeroMemory(&resource_view_desc, sizeof(resource_view_desc));
		resource_view_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		resource_view_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		resource_view_desc.Texture2D.MostDetailedMip = 0;
		resource_view_desc.Texture2D.MipLevels = 0xFFFFFFFF; // -1
		hr = RenderContext::get_device()->CreateShaderResourceView(m_texture, &resource_view_desc, &m_view);
		CHECK_HR(hr);

		RenderContext::get_context()->GenerateMips(m_view);

		set_sample_options({ Sample::Filter::Bilinear, Sample::Wrap::Repeat, Sample::Wrap::Repeat });
	}

	void Texture2D::cleanup()
	{
		SAFE_RELEASE(m_texture);
		SAFE_RELEASE(m_view);
		SAFE_RELEASE(m_sampler_state);
	}

	void Texture2D::update_from_memory(const void* data)
	{
		UINT raw_size = sizeof(char) * 4 * m_size.x;
		RenderContext::get_context()->UpdateSubresource(m_texture, NULL, NULL, data, raw_size, NULL);
		RenderContext::get_context()->GenerateMips(m_view);
	}

	void Texture2D::update_from_image(const Image* image)
	{
		update_from_memory(&(image->get_data())[0]);
	}

	void Texture2D::set_sample_options(const Sample& sample_options)
	{
		HRESULT hr;
		D3D11_SAMPLER_DESC sampler_desc;

		SAFE_RELEASE(m_sampler_state);

		ZeroMemory(&sampler_desc, sizeof(sampler_desc));
		sampler_desc.Filter = get_filter(sample_options.filter);
		sampler_desc.AddressU = get_wrap(sample_options.wrap_u);
		sampler_desc.AddressV = get_wrap(sample_options.wrap_v);
		sampler_desc.AddressW = get_wrap(Sample::Wrap::Clamp);
		sampler_desc.MipLODBias = 0.0f;
		sampler_desc.MaxAnisotropy = 1;
		sampler_desc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
		sampler_desc.BorderColor[0] = 0.0f;
		sampler_desc.BorderColor[1] = 0.0f;
		sampler_desc.BorderColor[2] = 0.0f;
		sampler_desc.BorderColor[3] = 1.0f;
		sampler_desc.MinLOD = 0.0f;
		sampler_desc.MaxLOD = sample_options.filter == Sample::Filter::Point ? sampler_desc.MinLOD : D3D11_FLOAT32_MAX;
		hr = RenderContext::get_device()->CreateSamplerState(&sampler_desc, &m_sampler_state);
		CHECK_HR(hr);
	}

	void Texture2D::use(const unsigned int slot)
	{
		RenderContext::get_context()->PSSetShaderResources(slot, 1, &m_view);
		RenderContext::get_context()->PSSetSamplers(slot, 1, &m_sampler_state);
	}

	// HELPER FUNCTIONS

	D3D11_FILTER get_filter(const Texture2D::Sample::Filter filter)
	{
		switch (filter)
		{
		case Texture2D::Sample::Filter::Point:
			return D3D11_FILTER_MIN_MAG_MIP_POINT;
		case Texture2D::Sample::Filter::Bilinear:
			return D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT;
		case Texture2D::Sample::Filter::Trilinear:
			return D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		case Texture2D::Sample::Filter::Anisotropic:
			return D3D11_FILTER_ANISOTROPIC;
		default:
			return D3D11_FILTER_MIN_MAG_MIP_POINT;
		}
	}

	D3D11_TEXTURE_ADDRESS_MODE get_wrap(const Texture2D::Sample::Wrap wrap)
	{
		switch (wrap)
		{
		case Texture2D::Sample::Wrap::Repeat:
			return D3D11_TEXTURE_ADDRESS_WRAP;
		case Texture2D::Sample::Wrap::Clamp:
			return D3D11_TEXTURE_ADDRESS_CLAMP;
		case Texture2D::Sample::Wrap::Mirror:
			return D3D11_TEXTURE_ADDRESS_MIRROR;
		case Texture2D::Sample::Wrap::MirrorOnce:
			return D3D11_TEXTURE_ADDRESS_MIRROR_ONCE;
		default:
			return D3D11_TEXTURE_ADDRESS_BORDER;
		}
	}
}