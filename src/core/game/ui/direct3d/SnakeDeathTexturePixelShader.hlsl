Texture2D Texture;
SamplerState ss;

// Pixel Shader
float4 main(float4 position : SV_POSITION, float4 color : COLOR, float2 texcoord : TEXCOORD) : SV_TARGET
{
	float4 ret = Texture.Sample(ss, texcoord);

	if (ret.a > 0.0)
	{
		float m = min(ret.r, ret.g);
		m = min(m, ret.b);
		float additive = (1.0 - m) * color.r;
		ret += additive;
	}

	if (color.a < 0)
	{
		color.a = 0;
	}

	color.r = 1;

	ret = ret * color;

	return ret;
}