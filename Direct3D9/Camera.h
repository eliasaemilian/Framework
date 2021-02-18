#pragma once
#include <d3d9.h>

class Camera
{
public:
	int init(INT screenWidth, INT screenHeight);
	void render(IDirect3DDevice9* pD3DDevice);
	void deInit();

private:
	D3DMATRIX _viewMatrix = {};
	D3DMATRIX _projectionMatrix = {};
};

