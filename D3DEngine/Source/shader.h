#pragma once
#include <string>
#include "renderdevice.h"
#include <memory>


class Shader
{
public:
	Shader();
	~Shader();

	void init(RenderDevice& targetdevice, const std::string& vertex, const std::string& pixel);
	void cleanup();

	void render(unsigned int indexCount);

private:
	std::shared_ptr<RenderDeviceImpl> m_devicehandle;
	struct ShaderBuffers;
	std::unique_ptr<ShaderBuffers> m_buffers;
};