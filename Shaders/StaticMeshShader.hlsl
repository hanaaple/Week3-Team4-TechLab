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
	float3 normal : NORMAL;
};

struct PS_OUTPUT
{
	float4 color : SV_Target;
};

float3 ComputeLighting(float3 normal, float3 lightDir, float3 viewDir, float3 lightColor, float3 specularColor, float specularExponent)
{
	float3 halfDir = normalize(lightDir + viewDir);

	// Diffuse Term
	float diff = max(dot(normal, lightDir), 0.0);

	// Specular Term
	float spec = 0.0;
	if (diff > 0.0)
	{
		spec = pow(max(dot(normal, halfDir), 0.0), specularExponent);
	}

	return (diff * Diffuse + spec * specularColor) * lightColor;
}



PS_INPUT mainVS(VS_INPUT input)
{
	PS_INPUT output;

	output.position = mul(float4(input.position.xyz, 1), MVP);
	//output.color = input.color;
	output.uv = input.uv;
	output.normal = input.normal;
	
	return output;
}

PS_OUTPUT mainPS(PS_INPUT input) : SV_Target
{
	PS_OUTPUT temp;
	
	// Sample textures
	float4 ambientColor = AmbientMap.Sample(g_Sample, input.uv);
	float4 diffuseColor = DiffuseMap.Sample(g_Sample, input.uv);
	float4 specularColor = SpecularMap.Sample(g_Sample, input.uv);
	float specularExponent = SpecularExponentMap.Sample(g_Sample, input.uv).r * SpecularExponent;
	float dissolve = DissolveMap.Sample(g_Sample, input.uv).r * Dissolve;
	
	temp.color = float4(diffuseColor);
	return temp;
	

	bool4 ambientBool = (ambientColor == 0.0);
	bool4 diffuseBool = (diffuseColor == 0.0);
	bool4 specularBool = (specularColor == 0.0);
	// If the texture is absent (sample value is (0, 0, 0, 0)), consider it as having no texture.
	if (all(ambientBool)) ambientColor = float4(1.0, 1.0, 1.0, 1.0);
	if (all(diffuseBool)) diffuseColor = float4(1.0, 1.0, 1.0, 1.0);
	if (all(specularBool)) specularColor = float4(0.0, 0.0, 0.0, 0.0);

	// Bump Mapping (Basic Normal Adjustment)
	float3 normal = normalize(float3(input.uv, 0));
	float3 bump = BumpMap.Sample(g_Sample, input.uv).rgb;
	normal = normalize(normal * bump);

	// Light and View Directions (Example Values)
	float3 lightDir = normalize(float3(0.0, -1.0, -1.0));
	float3 viewDir = normalize(-input.position.rgb);

	// Compute Lighting
	float3 lighting = ComputeLighting(normal, lightDir, viewDir, float3(1.0, 1.0, 1.0), specularColor.rgb, specularExponent);

	// Final Color with Emissive and Dissolve
	float3 finalColor = ambientColor.rgb * Ambient + diffuseColor.rgb * Diffuse * lighting + Emissive;

	// Apply Transparency
	if (dissolve < 0.1) // If dissolve is very low, the fragment is discarded
	{
		discard;
	}

	PS_OUTPUT output;
	output.color = float4(finalColor, dissolve);
	// Return final color
	return output;
	
	float threshold = 0.3f;
	float colorSum = input.color.r + input.color.g + input.color.b;
	
	if (colorSum < threshold)
		discard;
    
	return output;
}