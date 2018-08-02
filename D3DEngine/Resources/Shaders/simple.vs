
cbuffer MatrixBuffer
{
	matrix worldMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
};

struct VertexInputType
{
    float4 position : POSITION;
    float3 color : COLOR;
	float2 coord : COORD;
};

struct PixelInputType
{
    float4 position : SV_POSITION;
	float4 color : COLOR;
	float2 coord : COORD;
};


PixelInputType main(VertexInputType input)
{
    PixelInputType output;
    
	input.position.w = 1.0f;

    output.position = mul(input.position, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);

    output.color = float4(input.color, 1.0);
	output.coord = input.coord;

    return output;
}