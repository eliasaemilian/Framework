
cbuffer MatrixBuffer
{
    float4x4 WorldViewProjectionMatrix;
    float4x4 WorldMatrix;
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
    float4 localPos : POSITION;
};


VS_OUTPUT main( VS_INPUT IN )
{
    VS_OUTPUT OUT;
    
    float4 pos = IN.position;
    pos.w = 1.0f;
    
    OUT.position = mul( pos, WorldViewProjectionMatrix );
    OUT.position.z = OUT.position.w * 0.999;
    
    OUT.localPos = pos;
    
	return OUT;
}