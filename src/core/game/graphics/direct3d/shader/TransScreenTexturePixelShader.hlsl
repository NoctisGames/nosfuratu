cbuffer ProgressConstantBufferBuffer : register(b0)
{
    float progress;
};

cbuffer IsWindowsMobileConstantBufferBuffer : register(b1)
{
    int isWindowsMobile;
};

Texture2D TextureFrom;
Texture2D TextureTo;
SamplerState ss;

float rand(float2 co)
{
    return frac(sin(dot(co.xy, float2(12.9898, 78.233))) * 43758.5453);
}

// Pixel Shader
float4 main(float4 position : SV_POSITION, float2 texcoordIn : TEXCOORD) : SV_TARGET
{
    float prog = 1.0 - progress;
    float size = 0.15;
    float2 p = texcoordIn;
	float r;
    float m = 0.0;

    if (isWindowsMobile == 1)
    {
		r = rand(float2(p.x, 0));
        m = smoothstep(0.0, -size, p.y * (1.0 - size) + size * r - (prog * (1.0 + size)));
    }
    else
    {
		r = rand(float2(0, p.y));
        m = smoothstep(0.0, -size, p.x * (1.0 - size) + size * r - (prog * (1.0 + size)));
    }

    float4 ret = lerp(TextureTo.Sample(ss, p), TextureFrom.Sample(ss, p), m);

    return ret;
}