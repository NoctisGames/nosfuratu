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
	float smoothness = 0.1;
	float m = smoothstep(-smoothness, 0.0, 1.414213562373 * distance(center, p) - x * (1.5 + smoothness));
	float4 ret = lerp(TextureFrom.Sample(ss, p), TextureTo.Sample(ss, p), 1. - m);

	return ret;
}