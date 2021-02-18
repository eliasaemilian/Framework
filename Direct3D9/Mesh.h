#pragma once
#include <d3d9.h>

class Mesh
{
public:
	int init(IDirect3DDevice9* pD3DDevice);
	void update(float deltaTime);
	void render(IDirect3DDevice9* pD3DDevice);
	void deInit();

private:
	int initVertexBuffer(IDirect3DDevice9* pD3DDevice);
	int initIndexBuffer(IDirect3DDevice9* pD3DDevice);

	IDirect3DVertexBuffer9* _pVertexBuffer = nullptr;
	IDirect3DIndexBuffer9* _pIndexBuffer = nullptr;

	INT _vertexCount = 0; // amount of vertices
	INT _vertexStride = 0; // size of one vertex instance in bytes
	INT _indexCount = 0; // amount of indices

	D3DMATRIX _worldMatrix = {};
};

