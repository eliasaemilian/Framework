
cbuffer MatrixBuffer
{
    float4x4 WorldViewProjectionMatrix;
    float4x4 WorldMatrix;
    float4x4 ReflectionMatrix;
    float4 Time;
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
    OUT.position = mul(IN.position, WorldViewProjectionMatrix);
    OUT.uv = IN.uv;
    
    IN.normal.w = 0.0f;
    OUT.normal = mul(IN.normal, WorldMatrix).xyz;
    
    return OUT;
}