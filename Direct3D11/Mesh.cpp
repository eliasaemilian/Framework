#include "Mesh.h"
#include "Utils.h"

int Mesh::init( ID3D11Device* pD3DDevice )
{
	int error = initVertexBuffer( pD3DDevice );
	if (error != 0) return error;

	error = initIndexBuffer( pD3DDevice );
	if (error != 0) return error;


	return 0;
}

void Mesh::update( float deltaTime )
{
	static FLOAT posX = 0.0f;
	static FLOAT posZ = 0.0f;
	static FLOAT rotX = 0.0f;
	static FLOAT rotZ = 0.0f;

	float move = 1.0f * deltaTime;

	if (GetAsyncKeyState( VK_LEFT ) & 0x8000) posX -= move; // <- eeehm wtf???
	if (GetAsyncKeyState( VK_RIGHT ) & 0x8000) posX += move;
	if (GetAsyncKeyState( VK_UP ) & 0x8000) posZ += move;
	if (GetAsyncKeyState( VK_DOWN ) & 0x8000) posZ -= move;

	//rotX += 0.5f * deltaTime;
	//rotZ += 0.01f;

	XMMATRIX translation = XMMatrixTranslation( posX, 0.0f, posZ );
	XMMATRIX rotation = XMMatrixRotationRollPitchYaw( rotX, 0.0f, rotZ );
	XMMATRIX scale = XMMatrixScaling( 1.0f, 1.0f, 1.0f );

	//XMStoreFloat4x4(&_worldMatrix, scale * rotation * translation);
}

void Mesh::render( ID3D11DeviceContext* pD3DDeviceContext )
{
	static UINT offset = 0;

	pD3DDeviceContext->IASetVertexBuffers( 0, 1, &_pVertexBuffer, &_vertexStride, &offset );
	pD3DDeviceContext->IASetIndexBuffer( _pIndexBuffer, DXGI_FORMAT_R16_UINT, 0 );
	pD3DDeviceContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
	pD3DDeviceContext->DrawIndexed( _indexCount, 0, 0 );
}

void Mesh::deInit()
{
	safeRelease<ID3D11Buffer>( _pVertexBuffer );
	safeRelease<ID3D11Buffer>( _pIndexBuffer );
}

void Mesh::SetVertices( Vertex verts[], UINT count )
{

	for (UINT i = 0; i < count; i++)
	{
		_vertices.push_back( verts[i] );
	}

	_vertexCount = count;
}

void Mesh::SetVertices( std::vector<Vertex> vertices, UINT count )
{
	_vertices = vertices;
	_vertexCount = count;
}

void Mesh::SetIndices( WORD indices[], UINT count )
{
	for (UINT i = 0; i < count; i++)
	{
		_indices.push_back( indices[i] );
	}

	_indexCount = count;
}

void Mesh::SetIndices( std::vector<WORD> indices, UINT count )
{
	_indices = indices;
	_indexCount = count;
}

int Mesh::initVertexBuffer( ID3D11Device* pD3DDevice )
{
	_vertexStride = sizeof( Vertex );

	D3D11_BUFFER_DESC desc = {};
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	desc.ByteWidth = _vertexCount * _vertexStride;
	desc.Usage = D3D11_USAGE_IMMUTABLE;

	D3D11_SUBRESOURCE_DATA initialData = {};
	initialData.pSysMem = &_vertices[0], _vertices.size();


	HRESULT hr = pD3DDevice->CreateBuffer( &desc, &initialData, &_pVertexBuffer );
	if (FAILED( hr )) return 30;

	return 0;
}

int Mesh::initIndexBuffer( ID3D11Device* pD3DDevice )
{
	D3D11_BUFFER_DESC desc = {};
	desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	desc.ByteWidth = _indexCount * sizeof( WORD );
	desc.Usage = D3D11_USAGE_IMMUTABLE;

	D3D11_SUBRESOURCE_DATA initialData = {};
	initialData.pSysMem = &_indices[0], _indices.size();


	HRESULT hr = pD3DDevice->CreateBuffer( &desc, &initialData, &_pIndexBuffer );
	if (FAILED( hr )) return 35;

	return 0;
}
