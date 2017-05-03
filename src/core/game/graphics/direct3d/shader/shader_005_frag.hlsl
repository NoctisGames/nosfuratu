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

// Pixel Shader
float4 main(float4 position : SV_POSITION, float2 texcoordIn : TEXCOORD) : SV_TARGET
{
    float3 color = float3(0, 0, 0);
    float colorPhase = 0.4;

    float2 p = texcoordIn;

    float4 ret;

    if (progress > 0.4999 && progress < 0.5001)
    {
        ret = float4(0, 0, 0, 1);
    }
    else
    {
        ret = lerp(
                    lerp(float4(color, 1.0), TextureFrom.Sample(ss, p), smoothstep(1.0 - colorPhase, 0.0, progress)),
                    lerp(float4(color, 1.0), TextureTo.Sample(ss, p),   smoothstep(      colorPhase, 1.0, progress)),
                    progress);
    }

    return ret;
}
