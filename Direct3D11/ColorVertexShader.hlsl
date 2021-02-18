cbuffer MatrixBuffer
{
    float4x4 WorldViewProjectionMatrix;
};

struct VertexInput
{
    float4 position : POSITION;
    float4 color : COLOR;
};

struct VertexOutput
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
};

VertexOutput main(VertexInput IN)
{
    VertexOutput OUT;
    
    IN.position.w = 1.0f;
    OUT.position = mul(IN.position, WorldViewProjectionMatrix);
    OUT.color = IN.color;
    
	return OUT;
}