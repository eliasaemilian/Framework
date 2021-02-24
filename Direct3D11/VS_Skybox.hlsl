#define PI 3.14159265359

cbuffer Globals
{
    float4x4 CameraViewMatrix;
    float4x4 CameraProjectionMatrix;

    float4 Time;
};

cbuffer MaterialData 
{
    float4x4 WORLD_MATRIX;

    float4 TIMELAPSMOVEMENT; // X = [ TIMELAPS COUNTER VALUE ]
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

float3 RotateAroundYInDegrees( float3 vertex, float degrees )
{
    float alpha = degrees * PI / 180.0;
    float sina, cosa;
    sincos( alpha / 2, sina, cosa );
    float3x3 m = float3x3( cosa, 0, sina, 0, 1, 0, -sina, 0, cosa );
    float3 r = float3( mul( m, vertex.xyz ) ).rgb;
    return r;
}


VS_OUTPUT main( VS_INPUT IN )
{
    VS_OUTPUT OUT;
        
    float4 pos = IN.position;
    pos.w = 1.0f;
    
    // move skybox x axis to simulate passage of time        
    IN.position.xyz = RotateAroundYInDegrees( IN.position.xyz, TIMELAPSMOVEMENT);

      
    OUT.position = mul( IN.position, WORLD_MATRIX );
    OUT.position = mul( OUT.position, CameraViewMatrix );
    OUT.position = mul( OUT.position, CameraProjectionMatrix );
        
    OUT.position.z = OUT.position.w * 0.999; // putting this at close to end of the world
    
    
    OUT.localPos = pos;
    
	return OUT;
}