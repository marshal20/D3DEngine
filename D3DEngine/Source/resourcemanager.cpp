#include "resourcemanager.h"

#include <fstream>
#include <sstream>

#include "checks.h"
#include "3rdparty\stb_image\stb_image.h"
#include "3rdparty\tiny_obj_loader\tiny_obj_loader.h"

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
			checks::ENGINE_WRN(error_message.c_str());
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

	RawTextHandle loadTextFile(const std::string& filePath)
	{
		RawTextHandle rawTextHandle;
		std::ifstream inFile;
		std::stringstream contentsSS;

		inFile.open(filePath.c_str());
		if (inFile.is_open() == false)
		{
			std::string error_message = std::string("Open file failed: ") + filePath;
			checks::ENGINE_WRN(error_message.c_str());
		}

		std::string curLine;
		while (std::getline(inFile, curLine))
			contentsSS << curLine << '\n';
		contentsSS.flush();

		inFile.close();

		rawTextHandle.text = std::make_shared<std::string>();
		*rawTextHandle.text = contentsSS.str();

		return rawTextHandle;
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
			checks::ENGINE_WRN("Failed to load image data.");
		}

		rawImage.channels = 4;

		dataSize = sizeof(char) * rawImage.channels * rawImage.width * rawImage.height;
		rawImage.imageData.pData->resize(dataSize, 0);
		memcpy(&(*rawImage.imageData.pData)[0], dataPtr, dataSize);
		rawImage.imageData.size = dataSize;

		stbi_image_free(dataPtr);

		return rawImage;
	}

	Mesh loadMesh(const std::string& meshPath)
	{
		Mesh finalMesh;
		RawTextHandle rawOBJtext;
		std::stringstream OBJContents;
		tinyobj::attrib_t attrib;
		std::vector<tinyobj::shape_t> shapes;
		std::string err;

		rawOBJtext = loadTextFile(meshPath);

		OBJContents << *rawOBJtext.text;

		// tinyobj loading
		if (!tinyobj::LoadObj(&attrib, &shapes, nullptr, &err, &OBJContents))
		{
			std::string error_message = std::string("can't load OBJ file: ") + err;
			checks::ENGINE_WRN(error_message.c_str());
		}

		// reserve some space.
		finalMesh.vertexData.reserve(finalMesh.indexData.size());
		finalMesh.indexData.reserve(finalMesh.indexData.size());

		for (auto& index : shapes[0].mesh.indices)
		{
			int vertexIndex;
			int normalIndex;
			int texcoordIndex;

			finalMesh.vertexData.push_back(Mesh::Vertex());
			Mesh::Vertex& curVertex = finalMesh.vertexData.back();

			vertexIndex = index.vertex_index * 3;
			normalIndex = index.normal_index * 3;
			texcoordIndex = index.texcoord_index * 2;

			curVertex.position.x = attrib.vertices[vertexIndex + 0];
			curVertex.position.y = attrib.vertices[vertexIndex + 1];
			curVertex.position.z = attrib.vertices[vertexIndex + 2];
			
			curVertex.normal.x = attrib.normals[normalIndex + 0];
			curVertex.normal.y = attrib.normals[normalIndex + 1];
			curVertex.normal.z = attrib.normals[normalIndex + 2];
			
			curVertex.texCoord.x = attrib.texcoords[texcoordIndex + 0];
			curVertex.texCoord.y = attrib.texcoords[texcoordIndex + 1];

			finalMesh.indexData.push_back(finalMesh.vertexData.size()-1);
		}

		return finalMesh;
	}
}
