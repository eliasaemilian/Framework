#pragma once
#include "Mesh.h"
#include "Material.h"
#include "Camera.h"

class Gameobject
{
public:
	Gameobject();
	void init( ID3D11Device* dx, ID3D11DeviceContext* dCon, Camera* cam, Material* pMat, Mesh* pMesh );
	void render( ID3D11DeviceContext* dCon, Material::MaterialBuffer* mBuffer );
	void update( float deltatime );
	~Gameobject();

	// SETTERS
	void SetPosition( XMFLOAT3 position );
	void SetScale( XMFLOAT3 scale );
	void SetWorldMatrix();
	void SetRotation( XMFLOAT3 rotation );

	// GETTERS
	XMFLOAT3* getPosition() { return &_position; }
	XMFLOAT3* getRotation() { return &_rotation; }
	XMFLOAT3* getScale() { return &_scale; }

	XMFLOAT4X4* getWorldMatrix() { return &_worldMatrix; }

private:

	Mesh* _pMesh = nullptr;
	Material* _pMaterial = nullptr;
	Camera* _pCamera = nullptr;

	XMFLOAT3 _scale = XMFLOAT3( 1, 1, 1 );
	XMFLOAT3 _rotation = XMFLOAT3( 0, 0, 0 );
	XMFLOAT3 _position = XMFLOAT3( 0, 0, 0 );

	XMFLOAT4X4 _worldMatrix = {};

};

