struct PSInput
{
	float4 Position : SV_POSITION;
	float4 Color : COLOR;
};
    
float4 ShaderLine_PS(PSInput input) : SV_TARGET
{
	return input.Color;
}