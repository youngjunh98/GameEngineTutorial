struct VertexInput
{
	float3 position : POSITION;
	float2 uv : TEXCOORD;
	float3 normal : NORMAL;
};

struct VertexOutput
{
	float4 position : SV_POSITION;
	float2 uv : TEXCOORD;
	float3 normal : NORMAL;
};

VertexOutput VertexMain(VertexInput input)
{
	VertexOutput output;
    output.position = float4 (input.position, 1.0);
	output.uv = input.uv;
	output.normal = input.normal;

	return output;
}

float4 PixelMain(VertexOutput input) : SV_TARGET
{
	float NL = dot (input.normal, float3 (0.0, 0.0, -1.0));
	return float4 (NL, NL, NL, 1.0);
}