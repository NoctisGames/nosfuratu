Texture2D Texture;
SamplerState ss;

float SCurve(float x)
{
	x = x * 2.0 - 1.0;

	return -x * abs(x) * 0.5 + x + 0.5;
}

float4 BlurH(float2 uv, float radius)
{
	if (radius >= 1.0)
	{
		float4 A = float4(0.0, 0.0, 0.0, 0.0);
		float4 C = float4(0.0, 0.0, 0.0, 0.0);

		float width = 1.0 / 512.0;

		float divisor = 0.0;
		float weight = 0.0;

		float radiusMultiplier = 1.0 / radius;

		// Hardcoded for radius 20 (normally we input the radius
		// in there), needs to be literal here

		for (float x = -20.0; x <= 20.0; x++)
		{
			A = Texture.Sample(ss, uv + float2(x * width, 0.0));

			weight = SCurve(1.0 - (abs(x) * radiusMultiplier));

			C += A * weight;

			divisor += weight;
		}

		return float4(C.r / divisor, C.g / divisor, C.b / divisor, 1.0);
	}

	return Texture.Sample(ss, uv);
}

// Pixel Shader
float4 main(float4 position : SV_POSITION, float2 texcoord : TEXCOORD) : SV_TARGET
{
	// Apply horizontal blur to final output
	float4 ret = BlurH(texcoord, 20.0);
	ret.a = 0.5;

	return ret;
}
