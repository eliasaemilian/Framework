#pragma once
#include <vector>
#include <d3d11.h>
#include <DirectXMath.h>

using namespace DirectX;

class Skybox
{
public:
	void init( ID3D11Device* dev );
	void CreateSphere( ID3D11Device* dev, int LatLines, int LongLines );
	void update( float deltatime );
	void render( ID3D11DeviceContext* dev, XMFLOAT4X4* viewMatrix, XMFLOAT4X4* projectionMatrix );
	void deInit();

private:
	ID3D11Buffer* sphereIndexBuffer;
	ID3D11Buffer* sphereVertBuffer;

	ID3D11VertexShader* SKYMAP_VS;
	ID3D11PixelShader* SKYMAP_PS;
	ID3D10Blob* SKYMAP_VS_Buffer;
	ID3D10Blob* SKYMAP_PS_Buffer;

	ID3D11ShaderResourceView* smrv;

	ID3D11DepthStencilState* DSLessEqual;
	ID3D11RasterizerState* RSCullNone;

	int NumSphereVertices;
	int NumSphereFaces;

	XMMATRIX sphereWorld;

	XMMATRIX Rotationx; XMMATRIX Rotationy; XMMATRIX Rotationz;
};

