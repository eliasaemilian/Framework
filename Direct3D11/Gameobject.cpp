#include "Gameobject.h"

Gameobject::Gameobject() {}

void Gameobject::init( ID3D11Device* dx, ID3D11DeviceContext* dContext, Camera* cam, Material* pMaterial, Mesh* pMesh )
{
	XMStoreFloat4x4( &_worldMatrix, XMMatrixIdentity() );
	_scale = XMFLOAT3( 1.0f, 1.0f, 1.0f );
	_position = XMFLOAT3( 0.0f, 0.0f, 0.0f );
	_rotation = XMFLOAT3( 0.0f, 0.0f, 0.0f );
	SetWorldMatrix();

	_pMaterial = pMaterial;
	_pMesh = pMesh;
	_pCamera = cam;

}


void Gameobject::render( ID3D11DeviceContext* dCon, Material::MaterialBuffer* mBuffer )
{
	_pMaterial->render( dCon, mBuffer );
	_pMesh->render( dCon );
}


void Gameobject::SetPosition( XMFLOAT3 posIN )
{
	_position = posIN;
	SetWorldMatrix();

}

void Gameobject::SetScale( XMFLOAT3 scaleIN )
{
	_scale = scaleIN;
	SetWorldMatrix();
}

void Gameobject::SetWorldMatrix()
{
	XMMATRIX mPos = XMMatrixTranslation( _position.x, _position.y, _position.z );
	XMMATRIX mScale = XMMatrixScaling( _scale.x, _scale.y, _scale.z );
	CXMMATRIX mRot = XMMatrixRotationRollPitchYaw( _rotation.x, _rotation.y, _rotation.z );

	XMStoreFloat4x4( &_worldMatrix, mScale * mRot * mPos );

}

void Gameobject::SetRotation( XMFLOAT3 rotIN )
{
	_rotation = rotIN;
	SetWorldMatrix();
}

void Gameobject::update( float deltatime )
{
	_pMesh->update( deltatime );
}


Gameobject::~Gameobject() {}
