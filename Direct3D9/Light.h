#pragma once
#include <d3d9.h>

class Light
{
public:
	int init();
	void render(IDirect3DDevice9* pD3DDevice);
	void deInit();

private:
	D3DLIGHT9 _light = {};
};

