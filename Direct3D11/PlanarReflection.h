#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include <fstream>
#include "Material.h"

using namespace std;

class PlanarReflection : public Material
{
public:
	int initReflection( ID3D11Device* pD3DDevice );
	void renderReflection ( ID3D11DeviceContext* pD3DDeviceContext, ID3D11DepthStencilView* depthStencilView, XMFLOAT4X4* reflectionMatrix );

	ID3D11ShaderResourceView* getReflectionTexture() { return _pMainTexture; }


private:

	ID3D11RenderTargetView* m_renderTargetView; // <- this is where reflection gets rendered to OLD

	ID3D11Buffer* _pReflectionBuffer = nullptr;
	struct ReflectionBuffer
	{
		XMFLOAT4X4 reflectionMatrix;
	};
};

