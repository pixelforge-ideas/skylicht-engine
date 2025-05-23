Texture2D uTexture : register(t0);
SamplerState uTextureSampler : register(s0);

struct PS_INPUT
{
	float4 pos : SV_POSITION;
	float4 color : COLOR0;
	float2 tex0 : TEXCOORD0;
};

cbuffer cbPerFrame
{
	float4 uColorIntensity;
};

#include "../../PostProcessing/HLSL/LibToneMapping.hlsl"

float4 main(PS_INPUT input) : SV_TARGET
{
	float4 texColor = uTexture.Sample(uTextureSampler, input.tex0);	
	return float4(sRGB(input.color.rgb * input.color.a * texColor.r * uColorIntensity.rgb), 1.0);
}