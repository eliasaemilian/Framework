#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include <fstream>
#include "Material.h"

using namespace std;

class PlanarReflection 
{
public:
	void init( ID3D11Device* dev, UINT width, UINT height );
	void setPlanarReflection( ID3D11DeviceContext* devCon, XMFLOAT4X4* reflectionMatrix );
	void setRenderTargetToTexture( ID3D11DeviceContext* devCon, ID3D11DepthStencilView* depthStencilView );
	void clearRenderTargetView( ID3D11DeviceContext* devCon );

	void deinit();

	ID3D11RenderTargetView* getRenderTargetView() { return _pRenderTargetView; }


private:
	bool initReflectionTexture( ID3D11Device* dev, UINT width, UINT height );
	void initBuffer( ID3D11Device* dev );



private:
	ID3D11Buffer* _pReflectionBuffer = nullptr;
	struct ReflectionBuffer
	{
		XMFLOAT4X4 reflectionMatrix;
	};

	ID3D11Texture2D* _pRenderTargetTexture = nullptr;
	ID3D11RenderTargetView* _pRenderTargetView = nullptr;
	ID3D11ShaderResourceView* _pShaderResourceView = nullptr;


};

