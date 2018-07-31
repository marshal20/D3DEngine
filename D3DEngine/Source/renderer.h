#pragma once
#include "renderdevice.h"
#include "shader.h"
#include "pointerutil.h"
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
	ImplPtr<RenderDeviceImpl> m_devicehandle;
	struct RendererBuffers;
	ImplPtr<RendererBuffers> m_buffers;

	Shader m_shader;
};