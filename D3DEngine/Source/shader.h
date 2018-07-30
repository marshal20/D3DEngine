#pragma once
#include <string>
#include "renderdevice.h"
#include "pointerutil.h"
#include <d3d11.h>
#include <DirectXMath.h>

struct RenderDeviceImpl;

class Shader
{
public:
	Shader();
	~Shader();

	void init(RenderDevice* renderDevice, const std::string& vertex, const std::string& pixel);
	void cleanup();

	void render(unsigned int indexCount);

private:
	ImplPtr<RenderDeviceImpl> m_devicehandle;

	InterPtr<ID3D11VertexShader> m_vertexShader;
	InterPtr<ID3D11PixelShader> m_pixelShader;
	InterPtr<ID3D11InputLayout> m_layout;
	ID3D11Buffer* m_matrixBuffer;
};