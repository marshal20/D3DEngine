#pragma once

#include <memory>
#include <string>
#include <vector>

#include "mesh.h"

namespace Resource
{
	struct RawBinaryHandle
	{
		std::shared_ptr<std::vector<char>> pData;
		size_t size;
	};

	struct RawTextHandle
	{
		std::shared_ptr<std::string> text;
	};

	struct RawImageHandle
	{
		RawBinaryHandle imageData;
		int width;
		int height;
		int channels;
	};

	extern RawBinaryHandle loadFile(const std::string& filePath);
	extern RawTextHandle loadTextFile(const std::string& filePath);
	extern RawImageHandle loadImage(const std::string& imagePath);
	extern Mesh loadMesh(const std::string& meshPath);
}