cbuffer ProgressConstantBufferBuffer : register(b0)
{
    float progress;
};

Texture2D TextureFrom;
Texture2D TextureTo;
SamplerState ss;

// Pixel Shader
float4 main(float4 position : SV_POSITION, float4 color : COLOR, float2 texcoordIn : TEXCOORD0, float2 padding : TEXCOORD1, float2 centerIn : TEXCOORD2) : SV_TARGET
{
	float2 p = texcoordIn;
	float2 center = float2(centerIn.x, centerIn.y);
	float x = progress;

	float4 black = float4(0, 0, 0, 1);

	if (x <= 0.49)
	{
		x = x * 2.04081632653061;
		x = 1.0 - x;

		float smoothness = 0.1;
		float m = smoothstep(-smoothness, 0.0, 1.414213562373 * distance(center, p) - x * (1.5 + smoothness));
		return lerp(black, TextureFrom.Sample(ss, p), 1.0 - m);
	}
	else if (x >= 0.51)
	{
		x = x - 0.51;
		x = x * 2.04081632653061;

		float smoothness = 0.1;
		float m = smoothstep(-smoothness, 0.0, 1.414213562373 * distance(center, p) - x * (1.5 + smoothness));
		return lerp(black, TextureTo.Sample(ss, p), 1.0 - m);
	}
	else
	{
		return black;
	}
}
