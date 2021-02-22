
Texture2D MainTexture : register( t0 );
SamplerState MainSampler;

Texture2D NormalMap : register( t2 );
Texture2D RoughnessMap : register( t3 );


struct Light
{
    float3 LightDirection;
    float LightIntensity;
    float4 AmbientColor;
    float4 DiffuseColor;
};

cbuffer PixelShaderBuffer : register ( b0 )
{
    Light LightData;
};

struct PS_INPUT
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD;
    float3 normal : NORMAL;
    float4x4 worldMatrix : TEXCOORD1;
    float waterY : TEXCOORD5;
    float4 pos : TEXCOORD6;


};



float4 main(PS_INPUT IN) : SV_TARGET
{   
    float4 tex = MainTexture.Sample( MainSampler, IN.uv );
    float4 normal = NormalMap.Sample( MainSampler, IN.uv );
    float4 roughness = RoughnessMap.Sample( MainSampler, IN.uv );
    
        
    normal = mul( normal, IN.worldMatrix );
    normal = normalize( normal );
    
    
    // DIFFUSE LIGHTING
    
    //float diffuse = dot( -normalizedLight, normalizedNormal ); // calculate light intensity
    //diffuse = max( diffuse, 0.0f ); // dot product can be negative
    //diffuse *= LightData.lightIntensity; // adjust light intensity by multiplicator
    
    float3 lDir = - normalize( LightData.LightDirection );
    float lIntensity = saturate( dot( normal.xyz, lDir ) ) * LightData.LightIntensity; // get cos   
    
    
    float4 diRes = float4( saturate( LightData.DiffuseColor * lIntensity ).xyz, 1 );
    
   // return LightData.DiffuseColor;
    
    clip( ( IN.pos.y < IN.waterY ) ? -1 : 1 );
    
    return tex ;
}