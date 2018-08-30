#include <CoreEngine\texture\depthtexture.h>

#include <d3d11.h>

#include "..\state\internalstate.h"
#include "..\utils\callcheck.h"
#include "..\utils\safemem.h"

namespace ce
{
	DepthTexture::DepthTexture()
	{

	}

	DepthTexture::~DepthTexture()
	{

	}

	void DepthTexture::init(const math::Vec2<int>& size)
	{
		HRESULT hr;
		D3D11_TEXTURE2D_DESC depthBufferDesc;
		D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
		
		depthBufferDesc.Width = size.x;
		depthBufferDesc.Height = size.y;
		depthBufferDesc.MipLevels = 1;
		depthBufferDesc.ArraySize = 1;
		depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthBufferDesc.SampleDesc.Count = 1;
		depthBufferDesc.SampleDesc.Quality = 0;
		depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		depthBufferDesc.CPUAccessFlags = 0;
		depthBufferDesc.MiscFlags = 0;
		
		hr = state::d3d::m_device->CreateTexture2D(&depthBufferDesc, NULL, &m_texture);
		CHECK_HR(hr);
		
		ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));
		depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		depthStencilViewDesc.Texture2D.MipSlice = 0;
		
		hr = state::d3d::m_device->CreateDepthStencilView(m_texture, &depthStencilViewDesc, &m_view);
		CHECK_HR(hr);
	}

	void DepthTexture::cleanup()
	{
		SAFE_RELEASE(m_texture);
		SAFE_RELEASE(m_view);
	}

	ID3D11DepthStencilView* DepthTexture::get_view() const
	{
		return m_view;
	}
}
