#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include "Light.h"

#pragma comment(lib, "d3dcompiler.lib")

using namespace DirectX;

class Material
{
public:
	struct MaterialBuffer
	{
		XMFLOAT4X4 WORLD_MATRIX;

		XMFLOAT4 PARAM_FLOAT4_1;
		XMFLOAT4 PARAM_FLOAT4_2;
		XMFLOAT4 PARAM_FLOAT4_3;
		XMFLOAT4 PARAM_FLOAT4_4;
	};

public:
	int init(ID3D11Device* pD3DDevice, LPCWSTR textureName, LPCWSTR normalMap, LPCWSTR additionalTex, LPCWSTR  vertexShader, LPCWSTR pixelShader);
	virtual void render( ID3D11DeviceContext* pD3DDeviceContext, MaterialBuffer* mBuf );
	void deInit();

protected:
	// helper methods
	int createVertexShader(ID3D11Device* pD3DDevice, LPCWSTR vertexShader );
	int createPixelShader(ID3D11Device* pD3DDevice, LPCWSTR pixelShader );
	int createInputLayout(ID3D11Device* pD3DDevice, ID3DBlob* pBlob);
	int createMaterialBuffer(ID3D11Device* pD3DDevice );
	void setMaterialBuffer(ID3D11DeviceContext* pD3DDeviceContext, XMFLOAT4X4* mWorld, XMFLOAT4* f1, XMFLOAT4* f2, XMFLOAT4* f3, XMFLOAT4* f4 );
	virtual int createMainTextureAndSampler(ID3D11Device* pD3DDevice, LPCWSTR textureName );
	int createNormalMap(ID3D11Device* pD3DDevice, LPCWSTR textureName );
	int createAdditonalTexture(ID3D11Device* pD3DDevice, LPCWSTR textureName );
	int createCubeMapTextureAndSampler(ID3D11Device* pD3DDevice, LPCWSTR textureName);

	// shaders
	ID3D11VertexShader* _pVertexShader = nullptr;
	ID3D11PixelShader* _pPixelShader = nullptr;

	// input layout
	ID3D11InputLayout* _pInputLayout = nullptr;

	// textures and samplers
	ID3D11ShaderResourceView* _pMainTexture = nullptr;
	ID3D11SamplerState* _pMainSampler = nullptr;

	ID3D11ShaderResourceView* _pNormalMap = nullptr;
	ID3D11ShaderResourceView* _pAdditionalTex = nullptr;

	// BUFFERS
	ID3D11Buffer* _pMaterialBuffer = nullptr;
};

