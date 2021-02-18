
struct SKYMAP_VS_OUTPUT    //output structure for skymap vertex shader
{
    float4 Pos : SV_POSITION;
    float3 texCoord : TEXCOORD;
};


float4 main( float3 inPos : POSITION, float2 inTexCoord : TEXCOORD, float3 normal : NORMAL ) : SV_POSITION
{
    SKYMAP_VS_OUTPUT output = ( SKYMAP_VS_OUTPUT ) 0;

    //Set Pos to xyww instead of xyzw, so that z will always be 1 (furthest from camera)
    output.Pos = mul( float4( inPos, 1.0f ), WVP ).xyww;

    output.texCoord = inPos;

    return output;
}