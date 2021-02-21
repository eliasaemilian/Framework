Texture2D MainTexture;
SamplerState MainSampler;

Texture2D ReflectionTexture;


cbuffer Globals
{
    float4x4 CameraViewMatrix;
    float4x4 CameraProjectionMatrix;

    float4 Time;
};

cbuffer ReflectionMatrix
{
    float4x4 REFLECTION_MATRIX;
};


float3 WorldCamPos;

struct PS_INPUT
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD;
    float4 reflection : TEXCOORD1;
};


float4 main( PS_INPUT IN) : SV_TARGET
{
    float4 textureColor;
    float2 rUV;
    float4 rColor;
    float4 color;
    
    textureColor = MainTexture.Sample( MainSampler, IN.uv );
    // map UVS to 0 - 1 Range
    rUV.x = IN.reflection.x / IN.reflection.w / 2 + .5;
    rUV.y = IN.reflection.y / IN.reflection.w / 2 + .5;
    // get pixel pos with reflection uvs
    rColor = ReflectionTexture.Sample( MainSampler, rUV );
    
    // blend with rest
    color = lerp( textureColor, rColor, .15 ); // replace t with reflection strength
    
	return color;
}