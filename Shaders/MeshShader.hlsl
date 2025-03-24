Texture2D       g_txColor : register(t1);
SamplerState    g_Sample : register(s0);

cbuffer FConstantsComponentData : register(b0)
{
	float4x4 MVP;
};

struct VS_INPUT
{
	float3 position : POSITION; // Input position from vertex buffer
	float4 color : COLOR;       // Input color from vertex buffer
	float2 uv : TEXCOORD;
	float3 normal : NORMAL;
};

struct PS_INPUT
{
	float4 position : SV_POSITION; // Transformed position to pass to the pixel shader
	float4 color : COLOR;          // Color to pass to the pixel shader
	float2 uv : TEXCOORD;
};

struct PS_OUTPUT
{
	float4 color : SV_Target;
};


PS_INPUT mainVS(VS_INPUT input)
{
	PS_INPUT output;

	output.position = mul(float4(input.position.xyz, 1), MVP);
	output.color = input.color;
	output.uv = input.uv;
	
	return output;
}


PS_OUTPUT mainPS(PS_INPUT input)
{
	PS_OUTPUT output;

	output.color = g_txColor.Sample(g_Sample, input.uv);

	float threshold = 0.3f;
	float colorSum = input.color.r + input.color.g + input.color.b;
	
	if (colorSum < threshold)
		discard;
    
	return output;
}