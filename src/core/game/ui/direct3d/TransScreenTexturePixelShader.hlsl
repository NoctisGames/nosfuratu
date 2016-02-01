cbuffer ProgressConstantBufferBuffer : register(b0)
{
	float progress;
};

Texture2D TextureFrom;
Texture2D TextureTo;
SamplerState ss;

static const float2 center = float2(0.5, 0.5);
static const float SQRT_2 = 1.414213562373;

// Pixel Shader
float4 main(float4 position : SV_POSITION, float2 texcoordIn : TEXCOORD) : SV_TARGET
{
	float smoothness = 0.3;
	bool opening = true;
	float2 p = texcoordIn;
	float x = opening ? progress : 1. - progress;
	float m = smoothstep(-smoothness, 0.0, SQRT_2 * distance(center, p) - x * (1. + smoothness));
	float4 ret = lerp(TextureFrom.Sample(ss, p), TextureTo.Sample(ss, p), opening ? 1. - m : m);
	
	return ret;
}