#pragma once
#include <memory>

#include <DirectXMath.h>

#include "texture.h"


class Model
{
	struct VertexType
	{
		DirectX::XMFLOAT4 position;
		DirectX::XMFLOAT3 color;
		DirectX::XMFLOAT2 coord;
	};

public:
	Model();
	~Model();

	void init(Texture* texture);
	void cleanup();
	void bind() const;

private:
	friend class Renderer;
	int getIndexCount() const;

private:
	struct ModelBuffers;
	std::unique_ptr<ModelBuffers> m_buffers;
	int m_indexCount = 0;
	Texture* m_texture;
};