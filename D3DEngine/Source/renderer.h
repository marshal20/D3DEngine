#pragma once
#include "renderdevice.h"
#include "shader.h"
#include "pointerutil.h"

struct RenderDeviceImpl;

// TO BE DELETED
#include <d3d11.h>
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

	void init(RenderDevice* targetdevice);
	void cleanup();

	void render();

private:
	ImplPtr<RenderDeviceImpl> m_devicehandle;

	Shader m_shader;
	InterPtr<ID3D11Buffer> m_vertexBuffer;
	InterPtr<ID3D11Buffer> m_indexBuffer;
};