#include "resourcemanager.h"

#include <fstream>

#include "checks.h"
#include "3rdparty\stb_image\stb_image.h"

namespace Resource
{
	RawBinaryHandle loadFile(const std::string& filePath)
	{
		RawBinaryHandle rawBinaryFile;
		std::ifstream inFile;
		size_t fileSize;
		char* dataPtr;

		inFile.open(filePath.c_str(), std::ios::binary);
		if (inFile.is_open() == false)
		{
			std::string error_message = std::string("Open file failed: ") + filePath;
			ENGINE_ERROR(error_message.c_str());
		}

		// determine file size.
		inFile.seekg(0, std::ios::ios_base::end);
		fileSize = (size_t)inFile.tellg();
		inFile.seekg(0, std::ios::ios_base::beg);
		fileSize -= (size_t)inFile.tellg();

		rawBinaryFile.pData = std::make_shared<std::vector<char>>();

		rawBinaryFile.pData->resize(fileSize, 0);
		dataPtr = &((*rawBinaryFile.pData)[0]);

		inFile.read(dataPtr, fileSize);
		rawBinaryFile.size = fileSize;

		inFile.close();

		return rawBinaryFile;
	}

	RawImageHandle loadImage(const std::string& imagePath)
	{
		RawImageHandle rawImage;
		RawBinaryHandle fileData;
		char* dataPtr;
		size_t dataSize;

		fileData = loadFile(imagePath);

		rawImage.imageData.pData = std::make_shared<std::vector<char>>();

		dataPtr = (char*)stbi_load_from_memory((stbi_uc*)&(*fileData.pData)[0], fileData.size, 
			&rawImage.width, &rawImage.height, &rawImage.channels, 4);
		if (dataPtr == nullptr)
		{
			ENGINE_ERROR("Failed to load image data.");
		}

		rawImage.channels = 4;

		dataSize = sizeof(char) * rawImage.channels * rawImage.width * rawImage.height;
		rawImage.imageData.pData->resize(dataSize, 0);
		memcpy(&(*rawImage.imageData.pData)[0], dataPtr, dataSize);
		rawImage.imageData.size = dataSize;

		stbi_image_free(dataPtr);

		return rawImage;
	}
}
