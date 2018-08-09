#pragma once
#include <memory>

#include "resourcemanager.h"
#include "pointerutil.h"

struct ID3D11Texture2D;
struct ID3D11ShaderResourceView;
struct ID3D11DepthStencilView;

// Texture

class Texture
{
public:
	Texture(unsigned int width, unsigned int height, const char* data);
	Texture(const Texture& other);
	~Texture();

	static Texture* fromRaw(const Resource::RawImageHandle& imageHandle);

private:
	friend class Model;
	friend class RenderDevice;
	ID3D11ShaderResourceView* getView();

private:
	InterPtr<ID3D11Texture2D> m_pTexture;
	InterPtr<ID3D11ShaderResourceView> m_pView;
	int m_width, m_height;
};

// DepthStencilTexture

class DepthStencilTexture
{
public:
	DepthStencilTexture(unsigned int width, unsigned int height);
	DepthStencilTexture(const DepthStencilTexture& other);
	~DepthStencilTexture();

private:
	friend class Model;
	friend class RenderDevice;
	ID3D11DepthStencilView* getView();

private:
	InterPtr<ID3D11Texture2D> m_pTexture;
	InterPtr<ID3D11DepthStencilView> m_pView;
	int m_width, m_height;
};
