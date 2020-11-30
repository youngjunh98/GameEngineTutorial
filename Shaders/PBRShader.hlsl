cbuffer CBMatrix : register(b0)
{
    float4x4 g_LocalToWorld;
	float4x4 g_View;
	float4x4 g_Projection;
}

cbuffer CBRender : register(b1)
{
	float3 g_cameraPosition;
	float3 g_lightDirection;
	float3 g_lightColor; 
}

cbuffer CBPBR : register(b2)
{
	float g_roughness;
	float g_metallic;
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
	float2 uv : TEXCOORD0;
	float3 normal : NORMAL;
	float3 view : TEXCOORD1;
};

VertexOutput VertexMain(VertexInput input)
{
	VertexOutput output;
    output.position = mul (float4 (input.position, 1.0), g_LocalToWorld);
	float3 worldPosition = output.position.xyz;
	output.position = mul (output.position, g_View);
	output.position = mul (output.position, g_Projection);
	output.uv = input.uv;
	output.normal = normalize (mul (input.normal, (float3x3) g_LocalToWorld));
	output.view = normalize (g_cameraPosition - worldPosition);
	
	return output;
}

Texture2D BaseColorTexture;
SamplerState BaseColorTextureSampler;

float Diffuse_Lambert (float NdotL)
{
	return NdotL;
}

float D_GGX (float NdotH, float alpha)
{
	// Unity and "Course notes to moving Frostbite to PBR"
	// Apply division by PI later
	float a2 = alpha * alpha;
	float d = (NdotH * a2 - NdotH) * NdotH + 1.0;
	return a2 / (d * d);
}

float Vis_SmithCorrelated (float NdotL, float NdotV, float alpha)
{
	// "Course notes to moving Frostbite to PBR"
	// Height-Correlated Masking and Shadowing
	// GGX Distribution
	float a2 = alpha * alpha;
	float lambda_v = NdotL * sqrt ((-NdotV * a2 + NdotV) * NdotV + a2);
	float lambda_l = NdotV * sqrt ((-NdotL * a2 + NdotL) * NdotL + a2);
	return 0.5 / (lambda_v + lambda_l);
}

float Pow5 (float value)
{
	float v2 = value * value;
	return v2 * v2 * value;
}

float3 F_Schlick (float3 F0, float F90, float LdotH)
{
	// Unity and "Course notes to moving Frostbite to PBR"
	return F0 + (F90 - F0) * Pow5 (1 - LdotH);
}

#define PI 3.14159265359

float3 PBR_BRDF (float3 albedo, float3 specularColor, float roughness, float3 N, float3 V, float3 L)
{
	float NdotV = abs (dot (N, V)) + 0.00001;
	float NdotL = saturate (dot (N, L));
	
	float3 H = normalize (V + L);
	float NdotH = saturate (dot (N, H));
	float LdotH = saturate (dot (L, H));

	float D = D_GGX (NdotH, roughness);
	float Vis = Vis_SmithCorrelated (NdotL, NdotV, roughness);
	float3 F = F_Schlick (specularColor, 1.0, LdotH);

	float3 specular = D * F * Vis;
	float3 diffuse = albedo * Diffuse_Lambert (NdotL);

	return (diffuse + specular * NdotL) / PI;
}

float4 PixelMain(VertexOutput input) : SV_TARGET
{
	float3 baseColor = BaseColorTexture.Sample (BaseColorTextureSampler, input.uv).rgb;;
	float3 albedo = baseColor - baseColor * g_metallic;
	float3 specular = lerp (float(0.04).xxx, baseColor, g_metallic);
	float3 color = PBR_BRDF (albedo, specular, g_roughness, input.normal, input.view, normalize (-g_lightDirection));
	color *= g_lightColor * 10.0;
	return float4 (color, 1.0);
}