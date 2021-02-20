cbuffer MatrixBuffer
{
    float4x4 WorldViewProjectionMatrix;
    float4x4 WorldMatrix;
    float4x4 ReflectionMatrix;
    
    float4 Time;
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
    float4 reflection : TEXCOORD1;
};



VS_OUTPUT main( VS_INPUT IN ) 
{
    VS_OUTPUT OUT;
    
    
    OUT.position = mul( IN.position, WorldMatrix );
    OUT.position = mul( OUT.position, WorldViewProjectionMatrix );
    //OUT.position = mul( output.position, projectionMatrix );
    
    OUT.uv = IN.uv;
    
    // get world matrix for reflection
    float4x4 projectReflectWorld = mul( ReflectionMatrix, WorldViewProjectionMatrix );
    projectReflectWorld = mul( WorldMatrix, projectReflectWorld );

    // calculate positoin against PRW
    OUT.reflection = mul( IN.position, projectReflectWorld );
    
	return OUT;
}