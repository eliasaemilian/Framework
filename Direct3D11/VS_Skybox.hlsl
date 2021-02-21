
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



struct VS_INPUT
{
    float4 position : POSITION;
    float2 uv : TEXCOORD;
    float4 normal : NORMAL;
};



struct VS_OUTPUT
{
    float4 position : SV_POSITION;
    float4 localPos : POSITION;
};


VS_OUTPUT main( VS_INPUT IN )
{
    VS_OUTPUT OUT;
    
    float4 pos = IN.position;
    pos.w = 1.0f;
    
        
    OUT.position = mul( IN.position, WORLD_MATRIX );
    OUT.position = mul( OUT.position, CameraViewMatrix );
    OUT.position = mul( OUT.position, CameraProjectionMatrix );
        
    OUT.position.z = OUT.position.w * 0.999; // putting this at close to end of the world, camera doesnt move but like whatever
    
    OUT.localPos = pos;
    
	return OUT;
}