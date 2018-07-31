#include "shader.h"

#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#pragma comment(lib, "d3dcompiler.lib")

#include "checks.h"
#include "renderdeviceImpl.h"
#include "strutil.h"

struct Shader::ShaderBuffers
{
	InterPtr<ID3D11VertexShader> pVertexShader;
	InterPtr<ID3D11PixelShader> pPixelShader;
	InterPtr<ID3D11InputLayout> pLayout;
	InterPtr<ID3D11Buffer> pMatrixBuffer;
};

Shader::Shader()
{
	m_buffers = std::make_unique<ShaderBuffers>();
}

Shader::~Shader()
{

}

void Shader::init(RenderDevice& targetdevice, const std::string& vertex, const std::string& pixel)
{
	m_devicehandle = targetdevice.getImplementation();
	
	InterPtr<ID3D10Blob> vertexShaderBuffer;
	InterPtr<ID3D10Blob> pixelShaderBuffer;
	D3D11_INPUT_ELEMENT_DESC polygonLayout[1];
	unsigned int numElements;
	D3D11_BUFFER_DESC matrixBufferDesc;
	wchar_t* vsFilename;
	wchar_t* psFilename;

	vsFilename = create_wcharstr(vertex.c_str());
	psFilename = create_wcharstr(pixel.c_str());

	D3D11CALL(D3DCompileFromFile(vsFilename, NULL, NULL, "SimpleVertexShader", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0,
		&vertexShaderBuffer, NULL));
	D3D11CALL(D3DCompileFromFile(psFilename, NULL, NULL, "SimplePixelShader", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0,
		&pixelShaderBuffer, NULL));


	D3D11CALL(m_devicehandle->pDevice->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), 
		vertexShaderBuffer->GetBufferSize(), NULL, &m_buffers->pVertexShader));
	D3D11CALL(m_devicehandle->pDevice->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), 
		pixelShaderBuffer->GetBufferSize(), NULL, &m_buffers->pPixelShader));

	// setup the layout
	polygonLayout[0].SemanticName = "POSITION";
	polygonLayout[0].SemanticIndex = 0;
	polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[0].InputSlot = 0;
	polygonLayout[0].AlignedByteOffset = 0;
	polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[0].InstanceDataStepRate = 0;

	numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

	D3D11CALL(m_devicehandle->pDevice->CreateInputLayout(polygonLayout, numElements,
		vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), &m_buffers->pLayout));
	
	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(DirectX::XMMATRIX);
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;

	D3D11CALL(m_devicehandle->pDevice->CreateBuffer(&matrixBufferDesc, NULL, &m_buffers->pMatrixBuffer));

	delete[] vsFilename;
	delete[] psFilename;
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


	m_devicehandle->pContext->IASetInputLayout(m_buffers->pLayout);

	// Set the vertex and pixel shaders that will be used to render this triangle.
	m_devicehandle->pContext->VSSetShader(m_buffers->pVertexShader, NULL, 0);
	m_devicehandle->pContext->PSSetShader(m_buffers->pPixelShader, NULL, 0);

	// Render the triangle.
	m_devicehandle->pContext->DrawIndexed(indexCount, 0, 0);


}
