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
		XMFLOAT4X4 PARAM_MATRIX_1;
		XMFLOAT4X4 PARAM_MATRIX_2;
		XMFLOAT4X4 PARAM_MATRIX_3;
		XMFLOAT4X4 PARAM_MATRIX_4;

		XMFLOAT4 PARAM_FLOAT4_1;
		XMFLOAT4 PARAM_FLOAT4_2;
		XMFLOAT4 PARAM_FLOAT4_3;
		XMFLOAT4 PARAM_FLOAT4_4;
	};

public:
	int init(ID3D11Device* pD3DDevice, LPCWSTR textureName, LPCWSTR  vertexShader, LPCWSTR pixelShader);
	virtual void render( ID3D11DeviceContext* pD3DDeviceContext, XMFLOAT4X4* worldMatrix, XMFLOAT4X4* viewMatrix, XMFLOAT4X4* projectionMatrix, FLOAT time , MaterialBuffer* mBuf );
	void setLight(ID3D11DeviceContext* pD3DDeviceContext, Light& lightData);
	void deInit();

protected:
	// helper methods
	int createVertexShader(ID3D11Device* pD3DDevice, LPCWSTR vertexShader );
	int createPixelShader(ID3D11Device* pD3DDevice, LPCWSTR pixelShader );
	int createInputLayout(ID3D11Device* pD3DDevice, ID3DBlob* pBlob);
	int createMatrixBuffer(ID3D11Device* pD3DDevice);
	int createMaterialBuffer(ID3D11Device* pD3DDevice );
	void setMatrixBuffer(ID3D11DeviceContext* pD3DDeviceContext, XMFLOAT4X4* worldMatrix, XMFLOAT4X4* viewMatrix, XMFLOAT4X4* projectionMatrix, FLOAT time );
	void setMaterialBuffer(ID3D11DeviceContext* pD3DDeviceContext, XMFLOAT4X4* m1, XMFLOAT4X4* m2, XMFLOAT4X4* m3, XMFLOAT4X4* m4, XMFLOAT4* f1, XMFLOAT4* f2, XMFLOAT4* f3, XMFLOAT4* f4 );
	virtual int createTextureAndSampler(ID3D11Device* pD3DDevice, LPCWSTR textureName);
	int createCubeMapTextureAndSampler(ID3D11Device* pD3DDevice, LPCWSTR textureName);
	int createPixelShaderBuffer(ID3D11Device* pD3DDevice);

	// shaders
	ID3D11VertexShader* _pVertexShader = nullptr;
	ID3D11PixelShader* _pPixelShader = nullptr;

	// input layout
	ID3D11InputLayout* _pInputLayout = nullptr;

	// textures and samplers
	ID3D11ShaderResourceView* _pMainTexture = nullptr;
	ID3D11SamplerState* _pMainSampler = nullptr;

	// constant buffers
	ID3D11Buffer* _pMatrixBuffer = nullptr;
	struct MatrixBuffer
	{
		XMFLOAT4X4 worldViewProjectionMatrix;
		XMFLOAT4X4 worldMatrix;
		XMFLOAT4 time; // x -> Time.y, y -> deltaTime , zw -> padding
	};

	ID3D11Buffer* _pMaterialBuffer = nullptr;


	ID3D11Buffer* _pPixelShaderBuffer = nullptr;
	struct PixelShaderBuffer
	{
		Light lightData;
	};
};

