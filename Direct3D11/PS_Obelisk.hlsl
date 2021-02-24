
Texture2D MainTexture : register( t0 );
SamplerState MainSampler;

Texture2D NormalMap : register( t2 );
Texture2D RoughnessMap : register( t3 );


struct Light
{
    float3 lightDirection;
    float lightIntensity; // ambient intensity I guess ??
    float4 ambientColor;
    float4 diffuseColor;
};

cbuffer PixelShaderBuffer : register ( b0 )
{
    Light LightData;
};

cbuffer MaterialData : register( b2 )
{
    float4x4 WORLD_MATRIX;
    float4 PARAM_FLOAT4_2;
    float4 LIGHTING_ADJUSTS; // X [ AMBIENT INTENSITY ], Y [ DIFFUSE INTENSITY ], Z [ SPECULAR POWER ], W [ MODEL NORMAL STRENGTH ]
    float4 PARAM_FLOAT4_3; // x width, y height <- unused
    float4 PARAM_FLOAT4_4;
};

struct PS_INPUT
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD;
    float4 WorldPosWaterY : TEXCOORD1; // xyz = worldPos Object, w = waterY
    float3 viewDir : TEXCOORD2;
    float3 normal : TEXCOORD3;
};



float4 main(PS_INPUT IN) : SV_TARGET
{   
    // calc tangent and bitangent
    
     // compute derivations of the world position
    float3 p_dx = ddx( IN.WorldPosWaterY.xyz );
    float3 p_dy = ddy( IN.WorldPosWaterY.xyz );
    // compute derivations of the texture coordinate
    float2 tc_dx = ddx( IN.uv );
    float2 tc_dy = ddy( IN.uv );
    // compute initial tangent and bi-tangent
    float3 t = normalize( tc_dy.y * p_dx - tc_dx.y * p_dy );
    float3 b = normalize( tc_dy.x * p_dx - tc_dx.x * p_dy ); // sign inversion
    // get new tangent from a given mesh normal
    float3 n = normalize( IN.normal );
    float3 x = cross( n, t );
    t = cross( x, n );
    t = normalize( t );
    // get updated bi-tangent
    x = cross( b, n );
    b = cross( n, x );
    b = normalize( b );
    
    // sample textures
    float4 tex = MainTexture.Sample( MainSampler, IN.uv );
    float3 normal = NormalMap.Sample( MainSampler, IN.uv );
    float4 roughness = RoughnessMap.Sample( MainSampler, IN.uv );
    roughness = normalize( roughness );

    // calc normal from normal map
    float3 nModel = saturate( float4( IN.normal, 1 ) * LIGHTING_ADJUSTS.w ); // normal strength 
    float3 bump = (normal * 2) - 1;
    float3 bumpNormal = saturate( IN.normal + bump.x * t + bump.y * b );
    
    bumpNormal += nModel;   
    bumpNormal = normalize( saturate( bumpNormal ) );
    
    // lerp with model data 
    normal = normalize( lerp( nModel, bumpNormal, .2 ) );

    
    // LIGHTING     
    float ambientIntensity = LIGHTING_ADJUSTS.x;
    float diffuseIntensity = LIGHTING_ADJUSTS.y;
    float specularPower = LIGHTING_ADJUSTS.z;
    
    // init vars
    float4 final;
    float4 specular = float4( 0, 0, 0, 0 );
    float4 diffuse = float4( 0, 0, 0, 0 );   
    
    float3 lDir = normalize( LightData.lightDirection );
    float lIntensity = dot( normal, lDir );
    if ( lIntensity > 0 )
    {
        // calc diffuse using normals
        diffuse = saturate( LightData.diffuseColor * lIntensity );
        diffuse = saturate( float4( diffuse.xyz, 1 ) );

        // reflection vector
        float3 r = normalize( 2 * dot( lDir, normal ) * normal - lDir );
        
        // calc specular using roughness
        float d = dot( r, IN.viewDir );
        specular = max( pow( saturate( d ), specularPower ), 0 );
        specular = specular * roughness;
    } 
    // add up for final result
    final += diffuse;

    float4 col = saturate( tex + diffuse * diffuseIntensity );
    col.xyz += LightData.ambientColor * ambientIntensity;
    col.xyz *= LightData.lightIntensity;
    
    // clip reflection to cutoff below water surface
    clip( ( IN.WorldPosWaterY.y < IN.WorldPosWaterY.w ) ? -1 : 1 );

    return saturate( col + specular );
    
}