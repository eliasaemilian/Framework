#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include <fstream>
#include "Material.h"

using namespace std;

class PlanarReflection : public Material
{
public:
	virtual void render( ID3D11DeviceContext* pD3DDeviceContext, XMFLOAT4X4* reflectionMatrix, XMFLOAT4X4* viewMatrix, XMFLOAT4X4* projectionMatrix, FLOAT time, Material::MaterialBuffer* mBuffer ) override;

private:
	ID3D11Buffer* _pReflectionBuffer = nullptr;
	struct ReflectionBuffer
	{
		XMFLOAT4X4 reflectionMatrix;
	};
};

