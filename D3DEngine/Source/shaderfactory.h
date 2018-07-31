#pragma once
#include "shader.h"
#include <string>
#include "renderdevice.h"

namespace ShaderFactory
{
	extern Shader* getShader(const std::string& name);

	extern void addShader(RenderDevice& renderDevice, const std::string shaderName, const std::string& vertexpath, const std::string& pixelpath);
	extern void releaseShaders();
}
