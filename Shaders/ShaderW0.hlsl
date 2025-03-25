// ShaderW0.hlsl
//#pragma pack_matrix(column_major)

cbuffer FConstantsComponentData : register(b0)
{
	float4x4 MVP;
	float4 CustomColor;
	float4 UUIDColor;
	uint bUseVertexColor;
};



struct VS_INPUT
{
	float3 position : POSITION;  // 3개의 float 값 (DXGI_FORMAT_R32G32B32_FLOAT)
	float4 color : COLOR;        // 4개의 float 값 (DXGI_FORMAT_R32G32B32A32_FLOAT)
	float2 texcoord : TEXCOORD;  // 2개의 float 값 (DXGI_FORMAT_R32G32_FLOAT)
	float3 normal : NORMAL;      // 3개의 float 값 (DXGI_FORMAT_R32G32B32_FLOAT)
};

struct PS_INPUT
{
    float4 position : SV_POSITION; // Transformed position to pass to the pixel shader
    float4 color : COLOR;          // Color to pass to the pixel shader
    // float4 depthPosition : TEXCOORD0;
};

struct PS_OUTPUT
{
    float4 color : SV_Target0; //
	uint4 UUID : SV_Target1;
    //float depth : SV_Depth;
};


PS_INPUT mainVS(VS_INPUT input)
{
    PS_INPUT output;

	output.position = mul(float4(input.position.xyz, 1), MVP);
    // output.depthPosition = output.position;

    output.color = bUseVertexColor == 1 ? input.color : CustomColor;
    return output;
}


PS_OUTPUT mainPS(PS_INPUT input)
{
    PS_OUTPUT output;
	
    output.color = input.color;
    output.UUID = UUIDColor;
    // output.depth = saturate(depth);
    
    return output;
}

// float4 outlinePS(PS_INPUT input) : SV_TARGET
// {
//     // Output the color directly
//     return float4(1.0f, 0.647f, 0.0f, 0.1f);
// }

// PS_OUTPUT PickingPS(PS_INPUT input):SV_TARGET{
//     PS_OUTPUT output;
//     output.color = UUIDColor;
//     return output;
// }