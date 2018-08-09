#include "texture.h"

#include <d3d11.h>

#include "pointerutil.h"
#include "renderdevicehandle.h"
#include "checks.h"

// Texture

Texture::Texture(unsigned int width, unsigned int height, const char* data) :
	m_width(width), m_height(height)
{
	D3D11_TEXTURE2D_DESC textureDesc;
	D3D11_SHADER_RESOURCE_VIEW_DESC resourceViewDesc;
	size_t bufferSize;
	HRESULT hr;

	textureDesc.Width = m_width;
	textureDesc.Height = m_height;
	textureDesc.MipLevels = 0;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;

	hr = DeviceHandle::pDevice->CreateTexture2D(&textureDesc, nullptr, &m_pTexture);
	checks::D3D11CALL_WRN(hr, "ID3D11Device::CreateTexture2D(...) failed.");


	resourceViewDesc.Format = textureDesc.Format;
	resourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	resourceViewDesc.Texture2D.MostDetailedMip = 0;
	resourceViewDesc.Texture2D.MipLevels = 0xFFFFFFFF; // -1

	hr = DeviceHandle::pDevice->CreateShaderResourceView(m_pTexture, &resourceViewDesc, &m_pView);
	checks::D3D11CALL_WRN(hr, "ID3D11Device::CreateShaderResourceView(...) failed.");

	

	bufferSize = sizeof(char) * 4 * m_width;

	if (data) {
		DeviceHandle::pContext->UpdateSubresource(m_pTexture, NULL, nullptr, data, bufferSize, NULL);
	}

	DeviceHandle::pContext->GenerateMips(m_pView);
}

Texture::Texture(const Texture& other) :
	Texture(other.m_width, other.m_height, nullptr)
{
	DeviceHandle::pContext->CopyResource(m_pTexture, other.m_pTexture);
}

Texture::~Texture()
{
}

Texture* Texture::fromRaw(const Resource::RawImageHandle& imageHandle)
{
	return new Texture(imageHandle.width, imageHandle.height, &(*imageHandle.imageData.pData)[0]);
}

ID3D11ShaderResourceView* Texture::getView()
{
	return m_pView;
}

// DepthStencilTexture

DepthStencilTexture::DepthStencilTexture(unsigned int width, unsigned int height)
{
	D3D11_TEXTURE2D_DESC depthBufferDesc;
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	HRESULT hr;

	depthBufferDesc.Width = width;
	depthBufferDesc.Height = height;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;

	hr = DeviceHandle::pDevice->CreateTexture2D(&depthBufferDesc, NULL, &m_pTexture);
	checks::D3D11CALL_WRN(hr, "ID3D11Device::CreateTexture2D(...) failed.");

	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	hr = DeviceHandle::pDevice->CreateDepthStencilView(m_pTexture, &depthStencilViewDesc, &m_pView);
	checks::D3D11CALL_WRN(hr, "ID3D11Device::CreateDepthStencilView(...) failed.");
}

DepthStencilTexture::DepthStencilTexture(const DepthStencilTexture& other) :
	DepthStencilTexture(other.m_width, other.m_height)
{
	DeviceHandle::pContext->CopyResource(m_pTexture, other.m_pTexture);
}

DepthStencilTexture::~DepthStencilTexture()
{
}

ID3D11DepthStencilView* DepthStencilTexture::getView()
{
	return m_pView;
}