#include "shader.h"

#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#pragma comment(lib, "d3dcompiler.lib")

#include "checks.h"
#include "renderdevicehandle.h"
#include "pointerutil.h"
#include "strutil.h"

#include <vector>
#include <fstream>


struct Shader::ShaderBuffers
{
	InterPtr<ID3D11VertexShader> pVertexShader;
	InterPtr<ID3D11PixelShader> pPixelShader;
	InterPtr<ID3D11InputLayout> pLayout;
	InterPtr<ID3D11Buffer> pMatrixBuffer;
};

std::vector<D3D11_INPUT_ELEMENT_DESC> getLayout(const VertexLayout& layout);
void compileShaders(const char* vsname, const char* psname, ID3D10Blob** vsbuffer, ID3D10Blob** psbuffer);

Shader::Shader()
{
	m_buffers = std::make_unique<ShaderBuffers>();
}

Shader::~Shader()
{

}

void Shader::init(const std::string& vertex, const std::string& pixel, const VertexLayout& layout)
{	
	InterPtr<ID3D10Blob> vertexShaderBuffer;
	InterPtr<ID3D10Blob> pixelShaderBuffer;
	std::vector<D3D11_INPUT_ELEMENT_DESC> vertexLayout;
	D3D11_BUFFER_DESC matrixBufferDesc;

	compileShaders(vertex.c_str(), pixel.c_str(), &vertexShaderBuffer, &pixelShaderBuffer);
	
	D3D11CALL(DeviceHandle::pDevice->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(),
		vertexShaderBuffer->GetBufferSize(), NULL, &m_buffers->pVertexShader));
	D3D11CALL(DeviceHandle::pDevice->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(),
		pixelShaderBuffer->GetBufferSize(), NULL, &m_buffers->pPixelShader));

	vertexLayout = getLayout(layout);

	D3D11CALL(DeviceHandle::pDevice->CreateInputLayout(&vertexLayout[0], vertexLayout.size(),
		vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), &m_buffers->pLayout));
	//
	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(DirectX::XMMATRIX);
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;

	D3D11CALL(DeviceHandle::pDevice->CreateBuffer(&matrixBufferDesc, NULL, &m_buffers->pMatrixBuffer));

}

void Shader::cleanup()
{

}

void Shader::render(unsigned int indexCount)
{
	/*D3D11_MAPPED_SUBRESOURCE mappedResource;
	DirectX::XMMATRIX* dataPtr = new DirectX::XMMATRIX;
	unsigned int bufferNumber;


	// Transpose the matrices to prepare them for the shader.
	DirectX::XMMATRIX projection = DirectX::XMMatrixPerspectiveFovLH(3.14/4.0, 800.0/600.0, 0.1, 1000.0);

	// Lock the constant buffer so it can be written to.
	D3D11CALL(m_devicehandle->pContext->Map(m_matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource));

	// Get a pointer to the data in the constant buffer.
	dataPtr = (DirectX::XMMATRIX*)mappedResource.pData;

	// Copy the matrices into the constant buffer.
	*dataPtr = projection;

	// Unlock the constant buffer.
	m_devicehandle->pContext->Unmap(m_matrixBuffer, 0);

	// Set the position of the constant buffer in the vertex shader.
	bufferNumber = 0;

	// Finanly set the constant buffer in the vertex shader with the updated values.
	m_devicehandle->pContext->VSSetConstantBuffers(bufferNumber, 1, &m_matrixBuffer);
	*/


	DeviceHandle::pContext->IASetInputLayout(m_buffers->pLayout);

	// Set the vertex and pixel shaders that will be used to render this triangle.
	DeviceHandle::pContext->VSSetShader(m_buffers->pVertexShader, NULL, 0);
	DeviceHandle::pContext->PSSetShader(m_buffers->pPixelShader, NULL, 0);

	// Render the triangle.
	DeviceHandle::pContext->DrawIndexed(indexCount, 0, 0);


}


// HELPER FUNCTIONS

DXGI_FORMAT getFormatOfElement(const VertexLayout::Element& element)
{
	switch (element.type)
	{
	case VertexLayout::Type::FLOAT:
		switch (element.count)
		{
		case 1: return DXGI_FORMAT_R32_FLOAT;
		case 2: return DXGI_FORMAT_R32G32_FLOAT;
		case 3: return DXGI_FORMAT_R32G32B32_FLOAT;
		case 4: return DXGI_FORMAT_R32G32B32A32_FLOAT;
		default: return  DXGI_FORMAT_R32_TYPELESS;
		}

	case VertexLayout::Type::INT:
		switch (element.count)
		{
		case 1: return DXGI_FORMAT_R32_SINT;
		case 2: return DXGI_FORMAT_R32G32_SINT;
		case 3: return DXGI_FORMAT_R32G32B32_SINT;
		case 4: return DXGI_FORMAT_R32G32B32A32_SINT;
		default: return  DXGI_FORMAT_R32_TYPELESS;
		}
	default: return  DXGI_FORMAT_R32_TYPELESS;
	}
}

std::vector<D3D11_INPUT_ELEMENT_DESC> getLayout(const VertexLayout& layout)
{
	std::vector<D3D11_INPUT_ELEMENT_DESC> inputElementsLayout;

	inputElementsLayout.reserve(layout.elements.size());

	for (unsigned int i = 0; i < layout.elements.size(); i++)
	{
		D3D11_INPUT_ELEMENT_DESC curInputElement;
		const VertexLayout::Element& element = layout.elements[i];

		curInputElement.SemanticName = element.name.c_str();
		curInputElement.SemanticIndex = 0;
		curInputElement.Format = getFormatOfElement(element);
		curInputElement.InputSlot = 0;
		curInputElement.AlignedByteOffset = (i == 0) ? 0 : D3D11_APPEND_ALIGNED_ELEMENT;
		curInputElement.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		curInputElement.InstanceDataStepRate = 0;

		inputElementsLayout.push_back(curInputElement);
	}

	return inputElementsLayout;
}


void compileShaders(const char* vsname, const char* psname, ID3D10Blob** vsbuffer, ID3D10Blob** psbuffer)
{
	wchar_t* vsFilename;
	wchar_t* psFilename;
	InterPtr<ID3D10Blob> errorMessage;
	HRESULT hr;

	vsFilename = create_wcharstr(vsname);
	psFilename = create_wcharstr(psname);

	hr = D3DCompileFromFile(vsFilename, NULL, NULL, "main", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0,
		vsbuffer, &errorMessage);
	if (FAILED(hr))
	{
		ENGINE_ERROR((char*)errorMessage->GetBufferPointer(), hr);
	}

	hr = D3DCompileFromFile(psFilename, NULL, NULL, "main", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0,
		psbuffer, &errorMessage);
	if (FAILED(hr))
	{
		ENGINE_ERROR((char*)errorMessage->GetBufferPointer(), hr);
	}

	delete[] vsFilename;
	delete[] psFilename;
}
