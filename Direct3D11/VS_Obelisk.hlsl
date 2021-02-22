cbuffer Globals
{
    float4x4 CameraViewMatrix;
    float4x4 CameraProjectionMatrix;

    float4 Time;
};

cbuffer MaterialData
{
    float4x4 WORLD_MATRIX;

    float4 WATERY; // x = Water Y Position
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
    float4 normal : NORMAL;
    float4x4 worldMatrix : TEXCOORD1;
    float waterY : TEXCOORD5;
    float4 pos : TEXCOORD6;
};


VS_OUTPUT main( VS_INPUT IN )
{
    VS_OUTPUT OUT;
       
    // calc position against WVP matrices
    OUT.position = mul( IN.position, WORLD_MATRIX );
    OUT.pos = OUT.position;

    OUT.position = mul( OUT.position, CameraViewMatrix );
    OUT.position = mul( OUT.position, CameraProjectionMatrix );
    
    
    // calc normal against world matrix
    
    OUT.worldMatrix = WORLD_MATRIX;
    
    OUT.normal = mul( IN.normal, WORLD_MATRIX );
    OUT.normal = normalize( OUT.normal );
    
    // pass uvs
    OUT.uv = IN.uv;
    
    OUT.waterY = WATERY;
    
    return OUT;
}