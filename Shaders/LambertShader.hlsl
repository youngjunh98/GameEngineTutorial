cbuffer CBMatrix
{
    float4x4 g_LocalToWorld;
	float4x4 g_View;
	float4x4 g_Projection;
}

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
    output.position = mul (float4 (input.position, 1.0), g_LocalToWorld);
	output.position = mul (output.position, g_View);
	output.position = mul (output.position, g_Projection);
	output.uv = input.uv;
	output.normal = input.normal;
	
	return output;
}

Texture2D BaseColorTexture;
SamplerState BaseColorTextureSampler;

float4 PixelMain(VertexOutput input) : SV_TARGET
{
	float3 color = BaseColorTexture.Sample (BaseColorTextureSampler, input.uv).rgb;
	float NL = dot (input.normal, float3 (0.0, 0.0, -1.0));
	return float4 (color * NL, 1.0);
}