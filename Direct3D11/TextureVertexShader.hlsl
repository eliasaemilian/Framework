cbuffer MatrixBuffer
{
    float4x4 WorldViewProjectionMatrix;
    //float4 TilingOffset;
};

struct VertexInput
{
    float4 position : POSITION;
    float2 uv : TEXCOORD;
};

struct VertexOutput
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD;
};

VertexOutput main(VertexInput IN)
{
    VertexOutput OUT;
    
    IN.position.w = 1.0f;
    OUT.position = mul(IN.position, WorldViewProjectionMatrix);
    OUT.uv = IN.uv * float2(1.0f, 1.0f) + float2(0.0f, 0.0f); // uv * tiling + offset
    //OUT.uv = IN.uv * TilingOffset.xy + TilingOffset.zw; // uv * tiling + offset
    
    return OUT;
}