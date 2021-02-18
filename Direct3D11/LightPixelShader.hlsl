Texture2D MainTexture;
SamplerState MainSampler;

struct Light
{
    float3 LightDirection;
    float LightIntensity;
    float4 AmbientColor;
    float4 DiffuseColor;
};

cbuffer PixelShaderBuffer
{
    Light LightData;
};

struct PixelInput
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD;
    float3 normal : NORMAL;
};

float4 main(PixelInput IN) : SV_TARGET
{
    float4 mainTextureColor = MainTexture.Sample(MainSampler, IN.uv);
    
    float3 normalizedLight = normalize(LightData.LightDirection);
    float3 normalizedNormal = normalize(IN.normal);
    
    // diffuse light
    float diffuse = dot(-normalizedLight, normalizedNormal); // calculate light intensity
    diffuse = max(diffuse, 0.0f); // dot product can be negative
    diffuse *= LightData.LightIntensity; // adjust light intensity by multiplicator
    
    //return mainTextureColor * saturate(LightData.AmbientColor + LightData.DiffuseColor * diffuse);
    
    return float4( 0.5f, 0.5f, 0.5f, 1.0f );
}