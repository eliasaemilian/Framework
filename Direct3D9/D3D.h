#pragma once
#include <d3d9.h>

#pragma comment(lib, "d3d9.lib")

class D3D
{
public: 
	int init(HWND hWnd, INT width, INT height, bool isWindowed);
	void beginScene(D3DCOLOR backgroundColor);
	void endScene();
	void deInit();

	IDirect3DDevice9* getDevice() { return _pD3DDevice; }

private:
	// COM - Component Object Model
	IDirect3DDevice9* _pD3DDevice = nullptr;
};

