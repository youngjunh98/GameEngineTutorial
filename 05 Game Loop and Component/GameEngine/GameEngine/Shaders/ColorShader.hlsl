struct VertexInput
{
	float3 position : POSITION;
};

struct VertexOutput
{
	float4 position : SV_POSITION;
};

VertexOutput VertexMain(VertexInput input)
{
	VertexOutput output;
    output.position = float4 (input.position, 1.0);

	return output;
}

float4 PixelMain(VertexOutput input) : SV_TARGET
{
	return float4 (1.0, 0.0, 0.0, 1.0);
}