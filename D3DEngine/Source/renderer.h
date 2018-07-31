#pragma once
#include "renderdevice.h"
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

	void init(RenderDevice& targetdevice);
	void cleanup();

	void render();

private:
	std::shared_ptr<RenderDeviceImpl> m_devicehandle;
	struct RendererBuffers;
	std::unique_ptr<RendererBuffers> m_buffers;

	Shader* m_shader;
};