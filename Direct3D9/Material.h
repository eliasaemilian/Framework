#pragma once
#include <d3d9.h>

class Material
{
public:
	int init(IDirect3DDevice9* pD3DDevice, LPCWSTR textureName);
	void render(IDirect3DDevice9* pD3DDevice);
	void deInit();

private:
	IDirect3DTexture9* _pTexture = nullptr;
	D3DMATERIAL9 _material = {};
};

