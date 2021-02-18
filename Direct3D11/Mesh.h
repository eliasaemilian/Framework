#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include "Vertex.h"
#include <vector>

using namespace DirectX;

class Mesh
{
public:
	int init(ID3D11Device* pD3DDevice);
	void update(float deltaTime);
	void render(ID3D11DeviceContext* pD3DDeviceContext);
	void deInit();


	void SetVertices( Vertex vertices[], UINT count );
	void SetVertices( std::vector<Vertex> vertices, UINT count );
	void SetIndices( WORD indices[], UINT count );
	void SetIndices( std::vector<WORD> indices, UINT count );

private:
	int initVertexBuffer(ID3D11Device* pD3DDevice);
	int initIndexBuffer(ID3D11Device* pD3DDevice);

	ID3D11Buffer* _pVertexBuffer = nullptr;
	ID3D11Buffer* _pIndexBuffer = nullptr;

	UINT _vertexCount = 0; // amount of vertices
	UINT _vertexStride = 0; // size of one vertex instance in bytes
	UINT _indexCount = 0; // amount of indices


	std::vector<Vertex> _vertices = {};
	std::vector<WORD> _indices = {};


};

