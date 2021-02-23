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
    float3 viewDirection : TEXCOORD1;
    // float3 worldPos : TEXCOORD1;
};

float4 main(PixelInput IN) : SV_TARGET
{
    
    float4 albedo = float4( .1, .1, .1, 1 );

    float specularVal = 0.01;
    float powerVal = 0.01;
    
    float3 lightDirection = normalize( LightData.lightDirection );
    IN.normal = normalize( IN.normal );
    
    IN.viewDirection = normalize( IN.viewDirection );
    float diffuse = max( 0, dot( lightDirection, IN.normal ) );
    
    float3 reflectVector = 2.0 * diffuse * IN.normal - lightDirection;
    // add specular
    float specular = pow( max( 0, dot( reflectVector, IN.viewDirection ) ), specularVal ) * powerVal;
    
    
    // ambient and diffuse color
    float4 color = LightData.AmbientColor;
    color *= albedo;
    color += saturate( LightData.lightColor * diffuse );
    color += specular * float4( 1, 1, 1, 1 );
    
    return saturate( color );
    
    
   // float4 mainTextureColor = MainTexture.Sample(MainSampler, IN.uv);
    
   // float3 normalizedLight = normalize(LightData.lightDirection);
    
    
   // float3 normal = NormalMap.Sample( MainSampler, IN.uv ).xyz ;

   // float3 normalizedNormal = normalize(normal);
    
   // // diffuse light
   // float diffuse = dot(-normalizedLight, normalizedNormal); // calculate light intensity
   // diffuse = max(diffuse, 0.0f); // dot product can be negative
   // diffuse *= LightData.lightIntensity; // adjust light intensity by multiplicator
    
   // float4 res = mainTextureColor * saturate(LightData.AmbientColor + LightData.lightColor * diffuse);
    
   //// return float4( res.xyz, 1 );
   
    
   // float lightDir = normalize( LightData.lightDirection );
    
    
   // // Direct diffuse Light
   // float lightFalloff = saturate( dot( - lightDir, IN.normal ) );
   // float3 directDiffuseLight = LightData.lightColor * lightFalloff;
    
    //// Direct specular Light
    //float3 fragToCam = WorldCamPos - worldPos;
    
   // res =  ( directDiffuseLight, 1 );
    
  //  return float4( res.xyz, 1 );
}