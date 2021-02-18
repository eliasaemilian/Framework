Texture2D MainTexture;
SamplerState MainSampler;

struct PixelInput
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD;
};

float4 main(PixelInput IN) : SV_TARGET
{
    return MainTexture.Sample(MainSampler, IN.uv); // texture color
    //return MainTexture.Sample(MainSampler, IN.uv) * float4(1.0f, 0.0f, 0.0f, 1.0f); // texture color * main color
    //return 1.0f - MainTexture.Sample(MainSampler, IN.uv); // invert texture color
    //return MainTexture.Sample(MainSampler, IN.uv) * float4(0.0f, 1.0f, 1.0f, 1.0f); // texture color without red
    //return MainTexture.Sample(MainSampler, IN.uv) * 0.5f; // change texture color saturation
}