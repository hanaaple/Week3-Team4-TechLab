cbuffer FConstantsComponentData : register(b0)
{
	matrix ViewProjectionMatrix;
}


// 버텍스 셰이더
struct VS_INPUT
{
	float3 Position : POSITION;  // 3개의 float 값 (DXGI_FORMAT_R32G32B32_FLOAT)
	float4 Color : COLOR;        // 4개의 float 값 (DXGI_FORMAT_R32G32B32A32_FLOAT)
	float2 Texcoord : TEXCOORD;  // 2개의 float 값 (DXGI_FORMAT_R32G32_FLOAT)
	float3 normal : NORMAL;      // 3개의 float 값 (DXGI_FORMAT_R32G32B32_FLOAT)
};

struct VSOutput
{
	float4 Position : SV_POSITION;
	float4 Color : COLOR;
};


VSOutput ShaderLine_VS(VS_INPUT Input)
{

	VSOutput Output;
	Output.Position = mul(float4(Input.Position, 1.0f), ViewProjectionMatrix);
	Output.Color = Input.Color;
	return Output;
}