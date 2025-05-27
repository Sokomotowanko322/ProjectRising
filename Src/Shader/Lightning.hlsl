cbuffer TimeBuffer : register(b0)
{
    float time; // 時間
    float padding[3]; // 16バイトアライメントを確保
};

Texture2D logoTexture : register(t0);
SamplerState samplerState : register(s0);

float Random(float x)
{
    return frac(sin(x) * 43758.5453123);
}

float LightningEffect(float t)
{
    float flash = step(0.95, frac(sin(t * 10.0) * 10000.0));
    return flash * smoothstep(0.0, 1.0, sin(t * 20.0));
}

float4 main(float4 position : SV_POSITION, float2 texCoord : TEXCOORD) : SV_TARGET
{
    float4 baseColor = logoTexture.Sample(samplerState, texCoord);
    float flash = LightningEffect(time);
    float3 lightningColor = float3(1.0, 1.0, 1.0) * flash;
    return float4(baseColor.rgb + lightningColor, baseColor.a);
}