cbuffer IsWindowsMobileConstantBufferBuffer : register(b0)
{
	int isWindowsMobile;
};

cbuffer DirectionConstantBufferBuffer : register(b1)
{
	int direction;
};

Texture2D Texture;
SamplerState ss;

// Pixel Shader
float4 main(float4 position : SV_POSITION, float2 texcoordIn : TEXCOORD) : SV_TARGET
{
	const float strength = 0.125;
	const int samples = 8; // multiple of 2

	float2 fragCoord = texcoordIn;

	float2 uv = fragCoord.xy;

	float2 pos = float2(0, 0);
	if (isWindowsMobile == 1)
	{
		pos = float2(direction == 0 ? 0.5 : direction == 1 ? 0.0 : 1.0, 0.0);
	}
	else
	{
		pos = float2(0.0, direction == 0 ? 0.5 : direction == 1 ? 0.0 : 1.0);
	}
	
	float2 dir = (fragCoord.xy - pos.xy);

	float4 color = float4(0.0, 0.0, 0.0, 0.0);

	for (int i = 0; i < samples; i += 2) // operating at 2 samples for better performance
	{
		color += Texture.Sample(ss, uv + float(i) / float(samples) * dir * strength);
		color += Texture.Sample(ss, uv + float(i + 1) / float(samples) * dir * strength);
	}

	float4 ret = color / float(samples);

	return ret;
}