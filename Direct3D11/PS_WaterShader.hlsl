


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

cbuffer MaterialData : register ( b2 )
{
    float4x4 WORLD_MATRIX;

    float4 WATER_TRANSLATION; // x [ WATER TRANSLATION ]
    float4 REFRACTSCALE_TILING_SPEED; // X [ REFRACTION SCALE ], Y = [ REFRACTION TEXTURE TILING ], Z = [ REFRACTION SPEED]
    float4 SCREEN_WIDTH_HEIGHT; // x width, y height <- unused
    float4 PARAM_FLOAT4_4;
};

struct PixelInput
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD;
    float3 normal : NORMAL;
    float4 reflection : TEXCOORD1;
    float4 refraction : TEXCOORD2;
    float4 depthPos : TEXCOORD4;
};

float4 main( PixelInput IN ) : SV_TARGET
{    
    // INIT VARS
    float4 color = ( 1, 1, 1, 1 );
    float4 reflectionColor;
    float4 refractionColor;
    
    
    // REFRACTION / WATER CAUSTICS SIMULATION
    
    float reflectRefractScale = REFRACTSCALE_TILING_SPEED.x;
    float tiling = REFRACTSCALE_TILING_SPEED.y;
    float speed = REFRACTSCALE_TILING_SPEED.z;
    
    float2 rUV;

	// simulated water movement by moving uv sampling points	
    IN.uv.y += WATER_TRANSLATION.x * speed;
		
	// map UVS to 0 - 1 Range
    rUV.x =  IN.refraction.x / IN.refraction.w * 0.5f + 0.5f;
    rUV.y = -IN.refraction.y / IN.refraction.w * 0.5f + 0.5f;
    
    // sample normal map
    float4 normalMap = NormalMap.Sample( MainSampler, IN.uv );
    float3 normal = ( normalMap.xyz * 2.0f ) - 1.0f;
    
    rUV +=  normal.xy * reflectRefractScale ;

    // get pixel passing calculated refraction uvs
    refractionColor = MainTexture.Sample( MainSampler, rUV * tiling);
    
       
        
    // PLANAR REFLECTIONS
    
    // map UVS to 0 - 1 Range
    rUV.x = IN.reflection.x / IN.reflection.w * .5 + .5;
    rUV.y = -IN.reflection.y / IN.reflection.w * .5 + .5;

    // clamp uvs as to keep reflectin texture within screen params
    rUV.y += max( normal.y * reflectRefractScale, 0 );
    if ( rUV.x > .05 && rUV.x < .95 )
        rUV.x += normal.x * reflectRefractScale;

    
    // get pixel passing calculated reflection uvs
    reflectionColor = ReflectionTexture.Sample( MainSampler, rUV );
      
    // blend for final result
    color = saturate( lerp( refractionColor, reflectionColor, .65 ) ); // replace t with reflection strength
    
    color.xyz *= LightData.LightIntensity;


    return color;

}