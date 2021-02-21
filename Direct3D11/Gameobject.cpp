#include "Gameobject.h"

Gameobject::Gameobject()
{

}

void Gameobject::init( ID3D11Device* dx, ID3D11DeviceContext* dContext, Camera* cam, Material* pMaterial, Mesh* pMesh )
{
	XMStoreFloat4x4( &_worldMatrix, XMMatrixIdentity() );
	scale = XMFLOAT3( 1.0f, 1.0f, 1.0f );
	position = XMFLOAT3( 0.0f, 0.0f, 0.0f );
	rotation = XMFLOAT3( 0.0f, 0.0f, 0.0f );
	SetWorldMatrix();

	material = pMaterial;
	mesh = pMesh;
	camera = cam;

}


void Gameobject::render( ID3D11DeviceContext* dCon, Material::MaterialBuffer* mBuffer )
{
	material->render( dCon, mBuffer );
	mesh->render( dCon );
}


void Gameobject::SetPosition( XMFLOAT3 posIN )
{
	position = posIN;
	SetWorldMatrix();

}

void Gameobject::SetScale( XMFLOAT3 scaleIN )
{
	scale = scaleIN;
	SetWorldMatrix();
}

void Gameobject::SetWorldMatrix()
{
	XMMATRIX mPos = XMMatrixTranslation( position.x, position.y, position.z );
	XMMATRIX mScale = XMMatrixScaling( scale.x, scale.y, scale.z );
	CXMMATRIX mRot = XMMatrixRotationRollPitchYaw( rotation.x, rotation.y, rotation.z );

	XMStoreFloat4x4( &_worldMatrix, mScale * mRot * mPos );

}

void Gameobject::SetRotation( XMFLOAT3 rotIN )
{
	rotation = rotIN;
	SetWorldMatrix();
}

void Gameobject::update( float deltatime )
{
	mesh->update( deltatime );
}


Gameobject::~Gameobject()
{
	//mesh->deInit();
	//material->deInit();
}
