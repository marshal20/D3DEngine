#pragma once
#include "shader.h"
#include "shaderfactory.h"
#include <directxmath.h>


class Renderer
{
	struct VertexType
	{
		DirectX::XMFLOAT3 pos;
	};
public:
	Renderer();
	~Renderer();

	void init();
	void cleanup();

	void render();

private:
	struct RendererBuffers;
	std::unique_ptr<RendererBuffers> m_buffers;

	Shader* m_shader;
};