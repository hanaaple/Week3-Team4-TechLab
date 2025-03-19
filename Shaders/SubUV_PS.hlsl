Texture2D subUVTexture : register(t1);
SamplerState samLinear : register(s0);


cbuffer constants : register(b0)
{
	float4 UVOffsetAndSize;
	float4 AnimationParams;
	float4 ColorModifier;
}

struct VS_OUTPUT
{
	float4 Pos : SV_POSITION;
	float2 Tex : TEXCOORD;
};

float4 SubUV_PS(VS_OUTPUT input) : SV_TARGET
{
	float4 sampledColor = subUVTexture.Sample(samLinear, input.Tex);

	float threshold = 0.3f;
	float colorSum = sampledColor.r + sampledColor.g + sampledColor.b;
	
	return colorSum < threshold ? float4(1.0f, .0f, .0f, .0f) : sampledColor;
}