
cbuffer Globals
{
    float4x4 CameraViewMatrix;
    float4x4 CameraProjectionMatrix;

    float4 Time;
};

cbuffer MaterialData
{
    float4x4 WORLD_MATRIX;

    float4 PARAM_FLOAT4_1;
    float4 PARAM_FLOAT4_2;
    float4 PARAM_FLOAT4_3;
    float4 PARAM_FLOAT4_4;
};

struct VertexInput
{
    float4 position : POSITION;
    float2 uv : TEXCOORD;
    float4 normal : NORMAL;
};

struct VertexOutput
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD;
    float3 normal : NORMAL;
};



VertexOutput main(VertexInput IN)
{
    VertexOutput OUT;
    float _Amplitude = 1.0f;
    float _Wavelength = 10.0f;
    float _Speed = 5.0f;
    
    IN.position.w = 1.0f;
    float3 pos = IN.position.xyz;
    

    
    
    IN.position = float4( pos.xyz, IN.position.w );
    
    //float4x4 mvp = mul( WORLD_MATRIX, CameraViewMatrix );
    //mvp = mul( CameraProjectionMatrix, mvp );
    
    //OUT.position = mul(IN.position, mvp);
    
    OUT.position = mul( IN.position, WORLD_MATRIX );
    OUT.position = mul( OUT.position, CameraViewMatrix );
    OUT.position = mul( OUT.position, CameraProjectionMatrix );
    
    OUT.uv = IN.uv;
    
    IN.normal.w = 0.0f;
    OUT.normal = mul(IN.normal, WORLD_MATRIX).xyz;
    
    return OUT;
}