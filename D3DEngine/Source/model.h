#pragma once
#include <memory>

#include <DirectXMath.h>

class Model
{
	struct VertexType
	{
		DirectX::XMFLOAT4 position;
		DirectX::XMFLOAT3 color;
	};

public:
	Model();
	~Model();

	void init();
	void cleanup();
	void bind() const;

private:
	friend class Renderer;
	int getIndexCount() const;

private:
	struct ModelBuffers;
	std::unique_ptr<ModelBuffers> m_buffers;
	int m_indexCount = 0;
};