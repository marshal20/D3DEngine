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
};

enum class ShaderType {vs = 1, ps, gs};

std::vector<D3D11_INPUT_ELEMENT_DESC> getLayout(const VertexLayout& layout);
void compileShader(const char* contents, ID3D10Blob** ppShaderBuffer, ShaderType type);

Shader::Shader()
{
	m_buffers = std::make_unique<ShaderBuffers>();
}

Shader::~Shader()
{

}

void Shader::init(const std::string& vertexContents, const std::string& pixelContents, const VertexLayout& layout)
{	
	InterPtr<ID3D10Blob> vertexShaderBuffer;
	InterPtr<ID3D10Blob> pixelShaderBuffer;
	std::vector<D3D11_INPUT_ELEMENT_DESC> vertexLayout;
	HRESULT hr;

	compileShader(vertexContents.c_str(), &vertexShaderBuffer, ShaderType::vs);
	compileShader(pixelContents.c_str(), &pixelShaderBuffer, ShaderType::ps);
	
	hr = DeviceHandle::pDevice->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(),
		vertexShaderBuffer->GetBufferSize(), NULL, &m_buffers->pVertexShader);
	checks::D3D11CALL_WRN(hr, "ID3D11Device::CreateVertexShader(...) failed");

	hr = DeviceHandle::pDevice->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(),
		pixelShaderBuffer->GetBufferSize(), NULL, &m_buffers->pPixelShader);
	checks::D3D11CALL_WRN(hr, "ID3D11Device::CreatePixelShader(...) failed");

	vertexLayout = getLayout(layout);

	hr = DeviceHandle::pDevice->CreateInputLayout(&vertexLayout[0], vertexLayout.size(),
		vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), &m_buffers->pLayout);
	checks::D3D11CALL_WRN(hr, "ID3D11Device::CreateInputLayout(...) failed");
}

void Shader::cleanup()
{

}

void Shader::use()
{
	DeviceHandle::pContext->IASetInputLayout(m_buffers->pLayout);
	DeviceHandle::pContext->VSSetShader(m_buffers->pVertexShader, NULL, 0);
	DeviceHandle::pContext->PSSetShader(m_buffers->pPixelShader, NULL, 0);
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

const char* shaderTypeTochar(ShaderType type)
{
	switch (type)
	{
	case ShaderType::vs: return "vs_5_0";
	case ShaderType::ps: return "ps_5_0";
	case ShaderType::gs: return "gs_5_0";
	default: return "";
	}
}

void compileShader(const char* contents, ID3D10Blob** ppShaderBuffer, ShaderType type)
{
	InterPtr<ID3D10Blob> errorMessage;
	HRESULT hr;

	hr = D3DCompile(contents, strlen(contents), NULL, NULL, NULL, "main", shaderTypeTochar(type),
		D3D10_SHADER_ENABLE_STRICTNESS, 0, ppShaderBuffer, &errorMessage);
	if (FAILED(hr))
	{
		checks::ENGINE_WRN((char*)errorMessage->GetBufferPointer(), hr);
	}
}
