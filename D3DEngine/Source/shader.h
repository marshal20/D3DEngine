#pragma once
#include <string>
#include "renderdevice.h"
#include "pointerutil.h"


class Shader
{
public:
	Shader();
	~Shader();

	void init(RenderDevice& targetdevice, const std::string& vertex, const std::string& pixel);
	void cleanup();

	void render(unsigned int indexCount);

private:
	ImplPtr<RenderDeviceImpl> m_devicehandle;
	struct ShaderBuffers;
	ImplPtr<ShaderBuffers> m_buffers;
};