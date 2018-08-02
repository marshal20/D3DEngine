#pragma once

#include <memory>
#include <string>
#include <vector>

namespace Resource
{
	struct RawBinaryHandle
	{
		std::shared_ptr<std::vector<char>> pData;
		size_t size;
	};

	struct RawImageHandle
	{
		RawBinaryHandle imageData;
		int width;
		int height;
		int channels;
	};

	extern RawBinaryHandle loadFile(const std::string& filePath);
	extern RawImageHandle loadImage(const std::string& imagePath);
}