
cbuffer globalBuffer
{
	float4 posOff;
};

struct VertexInputType
{
    float4 position : POSITION;
    float3 color : COLOR;
};

struct PixelInputType
{
    float4 position : SV_POSITION;
	float4 color : COLOR;
};


PixelInputType main(VertexInputType input)
{
    PixelInputType output;
    
	output.position = input.position + posOff;
    output.color = float4(input.color, 1.0);
	
    return output;
}