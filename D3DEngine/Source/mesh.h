#pragma once
#include <vector>
#include <DirectXMath.h>

#include "mesh.h"

struct Mesh
{
	struct Vertex
	{
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT2 texCoord;
	};

	std::vector<Vertex> vertexData;
	std::vector<unsigned long> indexData;
};