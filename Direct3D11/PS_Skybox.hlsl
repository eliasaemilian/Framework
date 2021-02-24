
TextureCube cubeMap;

SamplerState skyboxSampler 
{
    Filter = MIN_MAG_LINEAR;
    AddressU = Wrap;
    AddressV = Wrap;
};

cbuffer MaterialData : register ( b2 )
{
    float4x4 WORLD_MATRIX;

    float4 TIMELAPSMOVEMENT; // X = [ TIMELAPS COUNTER VALUE ]
    float4 PARAM_FLOAT4_2;
    float4 PARAM_FLOAT4_3;
    float4 PARAM_FLOAT4_4;
};

struct PS_INPUT
{
    float4 position : SV_Position;
    float4 localPos : POSITION;
    float4 time : TEXCOORD0;

};


float4 main(PS_INPUT IN) : SV_TARGET
{
    float4 res = cubeMap.Sample( skyboxSampler, IN.localPos.xyz );
    return res;
}