
cbuffer MatrixBuffer
{
	matrix modelMatrix;
	matrix worldMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
};

struct VertexInputType
{
    float3 position : POSITION;
	float2 coord : COORD;
};

struct PixelInputType
{
    float4 position : SV_POSITION;
	float2 coord : COORD;
};


PixelInputType main(VertexInputType input)
{
    PixelInputType output;
    
	float4 position = float4(input.position, 1.0);

	output.position = mul(position, modelMatrix);
    output.position = mul(output.position, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);

	output.coord = input.coord;

    return output;
}