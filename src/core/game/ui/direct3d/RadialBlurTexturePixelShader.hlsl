Texture2D Texture;
SamplerState ss;

// Pixel Shader
float4 main(float4 position : SV_POSITION, float2 texcoordIn : TEXCOORD) : SV_TARGET
{
	const float strength = 0.125;
	const int samples = 16; // multiple of 2

	float2 fragCoord = texcoordIn;

	float2 uv = fragCoord.xy;

	float2 pos = float2(0.0, 0.5);
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