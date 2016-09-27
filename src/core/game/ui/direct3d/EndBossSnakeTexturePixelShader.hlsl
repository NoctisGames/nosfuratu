Texture2D Texture;
SamplerState ss;

// Pixel Shader
float4 main(float4 position : SV_POSITION, float4 color : COLOR, float2 texcoord : TEXCOORD) : SV_TARGET
{
	float4 ret = Texture.Sample(ss, texcoord);

	float4 colorOut = color;

	float k = colorOut.r - 1.0;

	k = clamp(k, 0.0, 2.0);

	colorOut.r = 1.0;
	colorOut.g = 1.0;
	colorOut.b = 1.0;

	if (k > 0.0)
	{
		if (ret.g > ret.r)
		{
			float originalR = ret.r;
			ret.r = ret.g * k;
			ret.g = originalR;

            ret.r = clamp(ret.r, 0.0, 1.0);
		}
	}

	ret = ret * colorOut;

	return ret;
}
