cbuffer TimeElapsedConstantBufferBuffer : register(b0)
{
	float timeElapsed;
};

Texture2D Texture;
Texture2D TextureGrayMap;
SamplerState ss;

// Pixel Shader
float4 main(float4 position : SV_POSITION, float2 texcoordIn : TEXCOORD) : SV_TARGET
{
	float2 uv = texcoordIn;
	float4 ret = Texture.Sample(ss, uv);

	// get gray comp of u_TextureUnitGrayMap
	float4 secondImageGray = TextureGrayMap.Sample(ss, uv);
	float gray = (secondImageGray.r + secondImageGray.g + secondImageGray.b) / 3.0;
	secondImageGray.r = gray;
	secondImageGray.g = gray;
	secondImageGray.b = gray;

	// threshold on sin time
	float thresh = (sin((timeElapsed + 2.6) * 1.6) * 0.5) + 0.5;

	if (secondImageGray.r > thresh)
	{
		ret.r = 0.0;
		ret.g = 0.0;
		ret.b = 0.0;
	}

	return ret;
}