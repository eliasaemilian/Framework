TextureCube SkyMap;
SamplerState ObjSamplerState;

struct SKYMAP_PS_INPUT    //output structure for skymap vertex shader
{
    float4 Pos : SV_POSITION;
    float3 texCoord : TEXCOORD;
};

float4 main( SKYMAP_PS_INPUT input ) : SV_TARGET
{
    return SkyMap.Sample( ObjSamplerState, input.texCoord );
}