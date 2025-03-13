cbuffer ConstantBuffer : register(b0)
{
	matrix ViewProjectionMatrix;
}

// 버텍스 셰이더
struct VSInput
{
	float3 Position : POSITION;
	float4 Color : COLOR;
};

struct VSOutput
{
	float4 Position : SV_POSITION;
	float4 Color : COLOR;
};


VSOutput ShaderLine_VS(VSInput Input)
{

	VSOutput Output;
	Output.Position = mul(float4(Input.Position, 1.0f), ViewProjectionMatrix);
	Output.Color = Input.Color;
	return Output;
}