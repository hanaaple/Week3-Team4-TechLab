Texture2D       AmbientMap : register(t1);
Texture2D       DiffuseMap : register(t2);
Texture2D       SpecularMap : register(t3);
Texture2D       SpecularExponentMap : register(t4);
Texture2D       DissolveMap : register(t5);
Texture2D       BumpMap : register(t6);

SamplerState    g_Sample : register(s0);

cbuffer FConstantsComponentData : register(b0)
{
	float4x4 MVP;
	float4 Color;
	float4 UUIDColor;
	uint bUseVertexColor;
	float4x4 M;
};

cbuffer MaterialData : register(b2)
{
	float SpecularExponent;			// Ns
	float3 Ambient;					// Ka
	float3 Diffuse;					// Kd
	float3 Specular;				// Ks
	float3 Emissive;				// Ke
	float OpticalDensity;			// Ni
	float Dissolve;					// d     Dissolve / Transparency
	int Illumination;				// illum
}

cbuffer CameraData : register(b3){
	float3 cameraPosition;
}

cbuffer Light : register(b4){
	float3 lightDirection;
	float3 lightDiffuseColor;
	float3 lightSpecularColor;
}

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
	float4 worldPosition : TEXCOORD1; // Transformed position to pass to the pixel shader
	float4x4 M : TEXCOORD2;
	float4 color : COLOR;          // Color to pass to the pixel shader
	float2 uv : TEXCOORD;
	float3 normal : NORMAL;
};

struct PS_OUTPUT
{
	float4 color : SV_Target;
};

PS_INPUT mainVS(VS_INPUT input)
{
	PS_INPUT output;

	output.position = mul(float4(input.position.xyz, 1), MVP);
	output.worldPosition = mul(float4(input.position.xyz, 1), M);
	output.M = M;
	output.color = input.color;
	output.uv = input.uv;
	output.normal = input.normal;
	
	return output;
}

PS_OUTPUT mainPS(PS_INPUT input) : SV_Target
{	
	// Sample textures
	float4 ambientColor = AmbientMap.Sample(g_Sample, input.uv);
	float4 diffuseColor = DiffuseMap.Sample(g_Sample, input.uv);
	float4 specularColor = SpecularMap.Sample(g_Sample, input.uv);
	float4 specularExponentColor = SpecularExponentMap.Sample(g_Sample, input.uv);
	float4 dissolveColor = DissolveMap.Sample(g_Sample, input.uv);
	float4 bumpColor = BumpMap.Sample(g_Sample, input.uv);

	// Tangent 계산
	//float3 tangent = normalize(mul(deltaPos1, deltaUV2.yx) - mul(deltaPos2, deltaUV1.yx));
	
	float3 L = mul(float4(lightDirection, 1), M).rgb;
	float3 N = normalize(input.normal);

	float3 viewDir = normalize(cameraPosition - input.worldPosition.rgb);
	float3 reflectDir = reflect(-L, N);
	
	float LDotN = dot(N, -L);

	float3 diffuse = diffuseColor.rgb * lightDiffuseColor * LDotN * Diffuse;

	float spec = pow(saturate(dot(reflectDir, viewDir)), SpecularExponent);
	float3 specular = spec * specularColor.rgb * lightSpecularColor * Specular;
	
	float4 finalColor = float4(diffuse + specular, 1);

	PS_OUTPUT output;
	output.color = finalColor;
	return output;
}