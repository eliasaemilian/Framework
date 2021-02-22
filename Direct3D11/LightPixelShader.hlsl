Texture2D MainTexture : register( t0 );
SamplerState MainSampler;

Texture2D NormalMap : register( t2 );

struct Light
{
    float3 lightDirection;
    float lightIntensity;
    float4 AmbientColor;
    float4 lightColor;
};



cbuffer PixelShaderBuffer
{
    Light LightData;
};

float3 WorldCamPos;

struct PixelInput
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD;
    float3 normal : NORMAL;
    // float3 worldPos : TEXCOORD1;
};

float4 main(PixelInput IN) : SV_TARGET
{
    float4 mainTextureColor = MainTexture.Sample(MainSampler, IN.uv);
    
    float3 normalizedLight = normalize(LightData.lightDirection);
    
    
    float3 normal = NormalMap.Sample( MainSampler, IN.uv ).xyz ;

    float3 normalizedNormal = normalize(normal);
    
    // diffuse light
    float diffuse = dot(-normalizedLight, normalizedNormal); // calculate light intensity
    diffuse = max(diffuse, 0.0f); // dot product can be negative
    diffuse *= LightData.lightIntensity; // adjust light intensity by multiplicator
    
    float4 res = mainTextureColor * saturate(LightData.AmbientColor + LightData.lightColor * diffuse);
    
    return float4( res.xyz, 1 );
   
    
    //float lightDir = normalize( LightData.lightDirection );
    
    //// Direct diffuse Light
    //float lightFalloff = saturate( dot( lightDir, IN.normal ) );
    //float3 directDiffuseLight = LightData.lightColor * lightFalloff;
    
    //// Direct specular Light
    //float3 fragToCam = WorldCamPos - worldPos;
    
   // return float4( 0.5f, 0.5f, 0.5f, 1.0f );
}