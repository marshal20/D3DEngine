#include "texture.h"

#include <d3d11.h>

#include "pointerutil.h"
#include "renderdevicehandle.h"
#include "checks.h"


struct Texture::TextureBuffers
{
	InterPtr<ID3D11Texture2D> pTexture;
	InterPtr<ID3D11ShaderResourceView> pView;
};

Texture::Texture()
{
	m_buffers = std::make_unique<TextureBuffers>();
}

Texture::~Texture()
{
	
}

void Texture::init(unsigned int width, unsigned int height, const char* data, int channels)
{
	m_width = width;
	m_height = height;
	m_channels = channels;

	D3D11_TEXTURE2D_DESC textureDesc;
	D3D11_SHADER_RESOURCE_VIEW_DESC resourceViewDesc;
	DXGI_FORMAT imageFormat;
	size_t bufferSize;

	imageFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
	switch (m_channels)
	{
	case 0: imageFormat = (DXGI_FORMAT)0; break;
	case 1: imageFormat = DXGI_FORMAT_R8_UNORM; break;
	case 2: imageFormat = DXGI_FORMAT_R8G8_UNORM; break;
	case 3: imageFormat = DXGI_FORMAT_R8G8_UNORM; break;
	case 4: imageFormat = DXGI_FORMAT_R8G8B8A8_UNORM; break;
	default: break;
	}

	textureDesc.Width = m_width;
	textureDesc.Height = m_height;
	textureDesc.MipLevels = 0;
	textureDesc.ArraySize = 1;
	textureDesc.Format = imageFormat;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;

	D3D11CALL(DeviceHandle::pDevice->CreateTexture2D(&textureDesc, nullptr, &m_buffers->pTexture));

	resourceViewDesc.Format = textureDesc.Format;
	resourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	resourceViewDesc.Texture2D.MostDetailedMip = 0;
	resourceViewDesc.Texture2D.MipLevels = 0xFFFFFFFF; // -1

	D3D11CALL(DeviceHandle::pDevice->CreateShaderResourceView(m_buffers->pTexture, &resourceViewDesc, &m_buffers->pView));

	bufferSize = sizeof(char) * m_channels * m_width;

	if (data)
	{
		DeviceHandle::pContext->UpdateSubresource(m_buffers->pTexture, NULL, nullptr, data, bufferSize, NULL);
	}

	DeviceHandle::pContext->GenerateMips(m_buffers->pView);
}

void Texture::cleanup()
{

}

Texture* Texture::fromRaw(const Resource::RawImageHandle& imageHandle)
{
	Texture* pTexture;

	pTexture = new Texture;

	pTexture->init(imageHandle.width, imageHandle.height, &(*imageHandle.imageData.pData)[0], imageHandle.channels);

	return pTexture;
}

void* Texture::getView()
{
	return m_buffers->pView;
}
