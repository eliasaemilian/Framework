
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

cbuffer ReflectionMatrix
{
    float4x4 REFLECTION_MATRIX;
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
    
    float4x4 mvp = mul( WORLD_MATRIX, CameraViewMatrix );
    mvp = mul( CameraProjectionMatrix, mvp );
    
    OUT.position = IN.position;
    
    OUT.position = mul( OUT.position, mvp );
    //OUT.position = mul( output.position, projectionMatrix );
    
    OUT.uv = IN.uv;
    
    // get world matrix for reflection
    float4x4 projectReflectWorld = mul( REFLECTION_MATRIX, CameraProjectionMatrix );
    projectReflectWorld = mul( WORLD_MATRIX, projectReflectWorld );

    // calculate positoin against PRW
    OUT.reflection = mul( IN.position, projectReflectWorld );
    
	return OUT;
}