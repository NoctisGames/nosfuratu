Texture2D Texture;
SamplerState ss;

// Pixel Shader
float4 main(float4 position : SV_POSITION, float4 color : COLOR, float2 texcoord : TEXCOORD) : SV_TARGET
{
	float4 ret = Texture.Sample(ss, texcoord);

	float4 colorOut = color;

	float k = 2.0 - colorOut.a;

	if (k >= 1.0)
	{
		k = 1.0;
		colorOut.r = 1.0;
		colorOut.g = 1.0;
		colorOut.b = 1.0;
	}

	colorOut.a = clamp(colorOut.a, 0.0, 1.0);

	if (ret.a > 0.0)
	{
		float m = min(ret.r, ret.g);
		m = min(m, ret.b);
		float additive = (1.0 - m) * k;
		ret += additive;
	}

	if (ret.r > 0.85
		&& ret.r < 1.0
		&& ret.g > 0.73
		&& ret.g < 0.88
		&& ret.b > 0.0
		&& ret.b < 0.15)
    {
		// We are sampling the Snake's Eye, make sure it stays Vampire Yellow!
    }
    else if (ret.r > 0.94
		&& ret.g > 0.94
		&& ret.b > 0.94)
    {
		// We are sampling the Snake's Fangs, make sure they stay White!
    }
    else
    {
		ret = ret * colorOut;
    }

	return ret;
}