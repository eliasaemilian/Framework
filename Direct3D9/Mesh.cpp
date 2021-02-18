#include "Mesh.h"
#include "Vertex.h"
#include <DirectXMath.h>

using namespace DirectX;

int Mesh::init(IDirect3DDevice9* pD3DDevice)
{
	int error = initVertexBuffer(pD3DDevice);
	if (error != 0) return error;

	error = initIndexBuffer(pD3DDevice);
	if (error != 0) return error;

	XMMATRIX identity = XMMatrixIdentity();
	XMFLOAT4X4 worldMatrix;
	XMStoreFloat4x4(&worldMatrix, identity);
	_worldMatrix = *reinterpret_cast<D3DMATRIX*>(&worldMatrix);

	return 0;
}

void Mesh::update(float deltaTime)
{
	static FLOAT posX = 0.0f;
	static FLOAT posZ = 0.0f;
	static FLOAT rotX = 0.0f;
	static FLOAT rotZ = 0.0f;

	float move = 1.0f * deltaTime;

	if (GetAsyncKeyState(VK_LEFT) & 0x8000) posX -= move;
	if (GetAsyncKeyState(VK_RIGHT) & 0x8000) posX += move;
	if (GetAsyncKeyState(VK_UP) & 0x8000) posZ += move;
	if (GetAsyncKeyState(VK_DOWN) & 0x8000) posZ -= move;

	rotX += 0.5f * deltaTime;
	//rotZ += 0.01f;

	XMMATRIX translation = XMMatrixTranslation(posX, 0.0f, posZ);
	XMMATRIX rotation = XMMatrixRotationRollPitchYaw(rotX, 0.0f, rotZ);
	XMMATRIX scale = XMMatrixScaling(1.0f, 1.0f, 1.0f);

	XMStoreFloat4x4(reinterpret_cast<XMFLOAT4X4*>(&_worldMatrix), scale * rotation * translation);
}

void Mesh::render(IDirect3DDevice9* pD3DDevice)
{
	pD3DDevice->SetTransform(D3DTS_WORLD, &_worldMatrix);

	pD3DDevice->SetFVF(FVF);
	pD3DDevice->SetStreamSource(0, _pVertexBuffer, 0, _vertexStride);
	//pD3DDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	// trianglelist with index buffer
	pD3DDevice->SetIndices(_pIndexBuffer);
	pD3DDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, _vertexCount, 0, _indexCount / 3);
}

void Mesh::deInit()
{
	if (_pVertexBuffer != nullptr)
	{
		_pVertexBuffer->Release();
		_pVertexBuffer = nullptr;
	}
}

int Mesh::initVertexBuffer(IDirect3DDevice9* pD3DDevice)
{
	_vertexCount = 4;
	_vertexStride = sizeof(Vertex);

	HRESULT hr = pD3DDevice->CreateVertexBuffer(
		_vertexCount * _vertexStride, // size of vertex buffer
		D3DUSAGE_WRITEONLY, // write and/or read the buffer
		FVF, // flexible vertex format
		D3DPOOL_MANAGED, // memory management
		&_pVertexBuffer,
		nullptr
	);
	if (FAILED(hr)) return 30;

	Vertex* pVertices = nullptr;
	hr = _pVertexBuffer->Lock(0, 0, reinterpret_cast<void**>(&pVertices), 0);
	if (FAILED(hr)) return 32;

	// triangle
	//pVertices[0] = Vertex(0.0f, 0.5f, 0.0f);
	//pVertices[1] = Vertex(0.5f, -0.5f, 0.0f);
	//pVertices[2] = Vertex(-0.5f, -0.5f, 0.0f);

	// quad - trianglelist without index buffer
	//// triangle 1
	//pVertices[0] = Vertex(-0.5f, 0.5f, 0.0f);
	//pVertices[1] = Vertex(0.5f, 0.5f, 0.0f);
	//pVertices[2] = Vertex(0.5f, -0.5f, 0.0f);
	//
	//// triangle 2
	//pVertices[3] = Vertex(-0.5f, -0.5f, 0.0f);
	//pVertices[4] = Vertex(-0.5f, 0.5f, 0.0f);
	//pVertices[5] = Vertex(0.5f, -0.5f, 0.0f);

	//// quad - trianglestrip or trianglelist with index buffer
	//pVertices[0] = Vertex(-0.5f, 0.5f, 0.0f, D3DCOLOR_XRGB(255, 0, 0));
	//pVertices[1] = Vertex(0.5f, 0.5f, 0.0f, D3DCOLOR_XRGB(0, 255, 0));
	//pVertices[2] = Vertex(-0.5f, -0.5f, 0.0f, D3DCOLOR_XRGB(0, 0, 255));
	//pVertices[3] = Vertex(0.5f, -0.5f, 0.0f, D3DCOLOR_XRGB(255, 0, 255));

	// quad - trianglestrip or trianglelist with index buffer
	// position with normal & uv
	pVertices[0] = Vertex(-0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f);
	pVertices[1] = Vertex(0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f);
	pVertices[2] = Vertex(-0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, -1.0f);
	pVertices[3] = Vertex(0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f);
	
	hr = _pVertexBuffer->Unlock();
	if (FAILED(hr)) return 34;

	pVertices = nullptr;

	return 0;
}

int Mesh::initIndexBuffer(IDirect3DDevice9* pD3DDevice)
{
	_indexCount = 6;

	HRESULT hr = pD3DDevice->CreateIndexBuffer(_indexCount * sizeof(WORD), D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, &_pIndexBuffer, nullptr);
	if (FAILED(hr)) return 36;

	WORD* indices = nullptr;
	hr = _pIndexBuffer->Lock(0, 0, reinterpret_cast<void**>(&indices), 0);
	if (FAILED(hr)) return 38;

	// quad
	indices[0] = 0; indices[1] = 1; indices[2] = 2;
	indices[3] = 1; indices[4] = 3; indices[5] = 2;

	hr = _pIndexBuffer->Unlock();
	if (FAILED(hr)) return 39;
	return 0;
}
