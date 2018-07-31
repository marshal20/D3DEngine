#pragma once
#include "shader.h"
#include <string>
#include "renderdevice.h"

namespace ShaderFactory
{
	extern Shader* getShader(const std::string& name);

	extern void addShader(const std::string shaderName, Shader* shaderPtr);
	extern void releaseShaders();
}
