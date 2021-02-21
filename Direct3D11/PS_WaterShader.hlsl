//Texture2D NormalMap;
Texture2D MainTexture;

SamplerState MainSampler;

Texture2D ReflectionTexture;


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

float4 main( PixelInput IN ) : SV_TARGET
{
    float4 mainTextureColor = ReflectionTexture.Sample( MainSampler, IN.uv );
    return mainTextureColor;
    
    
    
    float3 normal = normalize( MainTexture.Sample( MainSampler, IN.uv ).xyz );
    
    normal *= (  IN.normal );
    normal = normalize( normal );

    //return float4( normal, 1 );
    
    float4 albedo = float4( 0.133, 0.584, 0.827, 1 );
    float3 normalizedLight = normalize( LightData.LightDirection );
   // float3 normalizedNormal = normalize( mainTextureColor );
    
    // diffuse light
   // float diffuse = dot( -normalizedLight, normalizedNormal ); // calculate light intensity
    float diffuse = dot( -normalizedLight, normal ); // calculate light intensity
    diffuse = max( diffuse, 0.0f ); // dot product can be negative
    diffuse *= LightData.LightIntensity; // adjust light intensity by multiplicator
  
   // return float4( normalizedNormal, 1 );
    
    float3 lightDir = normalize( LightData.LightDirection );
    
    // Direct diffuse Light
    float lightFalloff = saturate( dot( lightDir, normal ) );
    lightFalloff *= LightData.LightIntensity;
    float3 directDiffuseLight = LightData.AmbientColor * lightFalloff;
    
    //return float4( directDiffuseLight, 1.0f );
    
    
    
    
    float4 res = albedo * saturate( LightData.AmbientColor + LightData.DiffuseColor * diffuse );
    res = albedo * saturate( float4( directDiffuseLight, 1 ) );

    return float4( res.xyz, .95 );
}