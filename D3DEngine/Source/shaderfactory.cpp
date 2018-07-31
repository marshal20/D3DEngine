#include "shaderfactory.h"

#include <map>


namespace ShaderFactory
{
	std::map<std::string, Shader*> i_shadersMap;


	Shader* getShader(const std::string& name)
	{
		auto found = i_shadersMap.find(name);
		if (found != i_shadersMap.end())
		{
			return found->second;
		}
		
		return nullptr;
	}

	void addShader(const std::string shaderName, Shader* shaderPtr)
	{
		auto newShader = std::make_pair(shaderName, shaderPtr);
		i_shadersMap.insert(newShader);
	}

	void releaseShaders()
	{
		for (auto item : i_shadersMap)
		{
			item.second->cleanup();
			delete item.second;
		}

		i_shadersMap.clear();
	}
}