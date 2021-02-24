
//Texture2D MainTexture : register( t0 );
//SamplerState MainSampler;

//Texture2D NormalMap : register( t2 );
//Texture2D RoughnessMap : register( t3 );


struct Light
{
    float3 LightDirection;
    float LightIntensity;
    float4 AmbientColor;
    float4 DiffuseColor;
};

cbuffer PixelShaderBuffer : register( b0 )
{
    Light LightData;
};

struct PS_INPUT
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD;
    float4x4 worldMatrix : TEXCOORD1;
    float4 worldPosCenter : TEXCOORD5;
    float4 worldPos : TEXCOORD6;
};



float4 main( PS_INPUT IN ) : SV_TARGET
{
    float3 albedo = ( 1, 1, 1);
    float alpha = .4;
    
    // adjusting albedo using uv to add globe effect
    float dist = distance( IN.worldPosCenter, IN.worldPos );
    float3 col = IN.uv.x * albedo;   
    col *= .5;
    
    // lerp for final result
    col = lerp( albedo, col, .5 ) * LightData.LightIntensity;
    
    return saturate( float4( col, alpha ) );
}