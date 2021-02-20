#define PI 3.14159265359

cbuffer MatrixBuffer
{
    float4x4 WorldViewProjectionMatrix;
    float4x4 WorldMatrix;

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



VertexOutput main( VertexInput IN )
{
    VertexOutput OUT;
    float _Amplitude = 0.2f;
    float _Amplitude2 = 0.01f;
    float _Wavelength = 2.0f;
    float _Wavelength2 = 1.0f;
    float _Speed = .4f;
    float _Speed2 = .3f;
    
    float2 _Direction = float2( .1, .6 );
    float2 _Direction2 = float2( .7, .2 );
    
    IN.position.w = 1.0f;
    float3 pos = IN.position.xyz;
    
    float k = 2 * ( float ) PI / _Wavelength;   
    float2 dir = normalize( _Direction );   
    float f = k * ( dot( dir, pos.xz ) - _Speed * Time );
    
    pos.y = _Amplitude * sin( f );
    
    
    float3 tangent = normalize( float3( 1, k * _Amplitude * cos( f ), 0 ) );
    
    float4 normal = float4( -tangent.y, tangent.x, 0, 0 );
    
    IN.position = float4( pos.xyz, IN.position.w );
    OUT.position = mul( IN.position, WorldViewProjectionMatrix );
    OUT.uv = IN.uv;
    
    IN.normal.w = 0.0f;
    OUT.normal = normalize( mul( normal, WorldMatrix ).xyz );
    
    return OUT;
}