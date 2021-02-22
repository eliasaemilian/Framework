


Texture2D MainTexture : register( t0 );
SamplerState MainSampler;

Texture2D ReflectionTexture : register( t1 );

Texture2D NormalMap : register( t2 );


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
    float4 reflection : TEXCOORD1;
};

float4 main( PixelInput IN ) : SV_TARGET
{
    
    // COLOR 
    
    float4 textureColor;
    float2 rUV;
    float4 rColor;
    float4 color;
    
    textureColor = MainTexture.Sample( MainSampler, IN.uv );
    // map UVS to 0 - 1 Range
    rUV.x = IN.reflection.x / IN.reflection.w / 2 + .5;
    rUV.y = - IN.reflection.y / IN.reflection.w / 2 + .5;
    // get pixel pos with reflection uvs
    rColor = ReflectionTexture.Sample( MainSampler, rUV );
    
    // blend with rest
    color = lerp( textureColor, rColor, .65 ); // replace t with reflection strength
    
    
    float3 normal = normalize( NormalMap.Sample( MainSampler, IN.uv ).xyz );
    
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
    
    
    
    
    float4 res = color * saturate( LightData.AmbientColor + LightData.DiffuseColor * diffuse );
    res = color * saturate( float4( directDiffuseLight, 1 ) );

    return float4( res.xyz, 1 );
}