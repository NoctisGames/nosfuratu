cbuffer TimeElapsedConstantBufferBuffer : register(b0)
{
	float timeElapsed;
};

cbuffer IsTransformingConstantBufferBuffer : register(b1)
{
	int isTransforming;
};

Texture2D Texture;
SamplerState ss;

// Pixel Shader
float4 main(float4 position : SV_POSITION, float4 color : COLOR, float2 texcoordIn : TEXCOORD0, float2 padding : TEXCOORD1, float2 centerIn : TEXCOORD2) : SV_TARGET
{
	float2 uv = texcoordIn;
	float2 center = float2(centerIn.x, centerIn.y);
	float2 texCoord = uv;
	float d = distance(uv, center);
	float time = timeElapsed;
	if ((d <= (time + 0.1)) && (d >= (time - 0.1)))
	{
		float diff = (d - time);
		float powDiff = 1.0 - pow(abs(diff * 10.0), 0.8);
		float diffTime = diff  * powDiff;
		float2 diffUV = normalize(uv - center);
		texCoord = uv + (diffUV * diffTime);
	}

	float4 ret = Texture.Sample(ss, texCoord);

	if (isTransforming == 1 && d < time)
	{
		ret.r *= 1.2;
		ret.b *= 1.1;
	}
	else if (isTransforming == 0 && d > time)
	{
		ret.r *= 1.2;
		ret.b *= 1.1;
	}

	return ret;
}