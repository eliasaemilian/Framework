
TextureCube cubeMap;

SamplerState skyboxSampler 
{
    Filter = MIN_MAG_LINEAR;
    AddressU = Wrap;
    AddressV = Wrap;
};


struct PS_INPUT
{
    float4 position : SV_Position;
    float4 localPos : POSITION;
};


float4 main(PS_INPUT IN) : SV_TARGET
{
    float4 res = cubeMap.Sample( skyboxSampler, IN.localPos.xyz );
    return res;
}