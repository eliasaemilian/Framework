cbuffer Globals
{
    float4x4 CameraViewMatrix;
    float4x4 CameraProjectionMatrix;

    float4 Time;
};

cbuffer MaterialData
{
    float4x4 WORLD_MATRIX;

    float4 CAMERAWORLDPOS_WATERY; // xyz = Camera World Pos, w = Water Y Position
    float4 WAVE_2;
    float4 PARAM_FLOAT4_3;
    float4 PARAM_FLOAT4_4;
};

struct VS_INPUT
{
    float4 position : POSITION;
    float2 uv : TEXCOORD;
    float4 normal : NORMAL;
};

struct VS_OUTPUT
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD;
    float4 worldPosWaterY : TEXCOORD1; // xyz = worldPos Object, w = waterY
    float3 viewDirection : TEXCOORD2;
    
    float3 normal : TEXCOORD3;
};


VS_OUTPUT main( VS_INPUT IN )
{
    VS_OUTPUT OUT;
    
    IN.position.w = 1;
    
    // calc position against WVP matrices
    OUT.position = mul( IN.position, WORLD_MATRIX );
    OUT.position = mul( OUT.position, CameraViewMatrix );
    OUT.position = mul( OUT.position, CameraProjectionMatrix );      
    
    // pass uvs
    OUT.uv = IN.uv;
     
    // calc view direction 
    float4 worldPos = mul( IN.position, WORLD_MATRIX );
    OUT.viewDirection = CAMERAWORLDPOS_WATERY.xyz - worldPos.xyz;
    OUT.viewDirection = normalize( OUT.viewDirection );
    
    // pass world position, water param   
    OUT.worldPosWaterY.xyz = worldPos.xyz;
    OUT.worldPosWaterY.w = CAMERAWORLDPOS_WATERY.w;
    
    // calc normal
    OUT.normal = mul( IN.normal, WORLD_MATRIX );
    OUT.normal = normalize( OUT.normal );
        
    
    return OUT;
}