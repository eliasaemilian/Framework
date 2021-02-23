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

    float4 WAVE_1;
    float4 WAVE_2;
    float4 SCREEN_WIDTH_HEIGHT; // x width, y height
    float4 PARAM_FLOAT4_4;
};

cbuffer ReflectionMatrix
{
    float4x4 REFLECTION_MATRIX;
};

struct VertexInput
{
    float4 position : POSITION;
    float2 uv : TEXCOORD;
    float4 normal : NORMAL;
};

struct VertexOutput
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD;
    float3 normal : NORMAL;
    float4 reflection : TEXCOORD1;
    float4 refraction : TEXCOORD2;
    float waterTranslation : TEXCOORD3;
    float4 position2 : TEXCOORD4;
};



VertexOutput main( VertexInput IN )
{
    VertexOutput OUT;
    
    OUT.uv = IN.uv;
    OUT.normal = IN.normal;
    
    OUT.waterTranslation = Time.w;
    
    IN.position.w = 1;
    
    // REFRACTION
    float4x4 reflectProjectWorld;
    float4x4 viewProjectWorld;
    
    // Create the view projection world matrix for refraction.
    viewProjectWorld = mul( CameraViewMatrix, CameraProjectionMatrix );
    viewProjectWorld = mul( WORLD_MATRIX, viewProjectWorld );
   
	// Calculate the input position against the viewProjectWorld matrix.
   // OUT.refraction = mul( IN.position, viewProjectWorld ).xyz;
    
    
        
    OUT.refraction = mul( IN.position, WORLD_MATRIX );
    OUT.refraction = mul( OUT.refraction, CameraViewMatrix );
    OUT.refraction = mul( OUT.refraction, CameraProjectionMatrix );
    
    //OUT.position = IN.position;
    //OUT.normal = IN.normal;
    //OUT.uv = IN.uv;
    //return OUT;
    
    // calc reflection uvs
    
    // get world matrix for reflection
    float4x4 projectReflectWorld = mul( REFLECTION_MATRIX, CameraProjectionMatrix );
    projectReflectWorld = mul( WORLD_MATRIX, projectReflectWorld );

    // calculate positoin against PRW
    //OUT.reflection = mul( IN.position, projectReflectWorld );
    
    OUT.reflection = mul( IN.position, WORLD_MATRIX );
    OUT.reflection = mul( OUT.reflection, REFLECTION_MATRIX );
    OUT.reflection = mul( OUT.reflection, CameraProjectionMatrix );
    
    //reflectProjectWorld = mul( REFLECTION_MATRIX, CameraProjectionMatrix );
    //reflectProjectWorld = mul( WORLD_MATRIX, reflectProjectWorld );
    //OUT.reflection = mul( IN.position, reflectProjectWorld );

    
    // END
    
    
    //float _Amplitude = 0.2f;
    //float _Amplitude2 = 0.01f;
    //float _Wavelength = 2.0f;
    //float _Wavelength2 = 1.0f;
    //float _Speed = .4f;
    //float _Speed2 = .3f;
    
    //float2 _Direction = float2( .1, .6 );
    //float2 _Direction2 = float2( .7, .2 );
    
    //IN.position.w = 1.0f;
    //float3 pos = IN.position.xyz;
    
    //float k = 2 * ( float ) PI / _Wavelength;   
    //float2 dir = normalize( _Direction );   
    //float f = k * ( dot( dir, pos.xz ) - _Speed * Time );
    
    //pos.y = _Amplitude * sin( f );
    
    
    //float3 tangent = normalize( float3( 1, k * _Amplitude * cos( f ), 0 ) );
    
    //float4 normal = float4( -tangent.y, tangent.x, 0, 0 );
    
    ////IN.position = float4( pos.xyz, IN.position.w );
    
    OUT.position = mul( IN.position, WORLD_MATRIX );
    OUT.position = mul( OUT.position, CameraViewMatrix );
    OUT.position = mul( OUT.position, CameraProjectionMatrix );
    
    OUT.position2 = OUT.position;
    //OUT.uv = IN.uv;
    
    //IN.normal.w = 0.0f;
    //OUT.normal = normalize( mul( normal, WORLD_MATRIX ).xyz );
    
    return OUT;
}