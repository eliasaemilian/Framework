#pragma once
#include "Mesh.h"
#include "Material.h"
#include "Camera.h"

class Gameobject
{
public:
	Gameobject();
	void init( ID3D11Device* dx, ID3D11DeviceContext* dCon, Camera* cam, Material* pMat, Mesh* pMesh );
	~Gameobject();
	void render( ID3D11DeviceContext* dCon, Material::MaterialBuffer* mBuffer );
	void SetPosition( XMFLOAT3 position );
	void SetScale( XMFLOAT3 scale );
	void SetWorldMatrix();
	void SetRotation( XMFLOAT3 rotation );
	void update( float deltatime );

	XMFLOAT4X4* getWorldMatrix() { return &_worldMatrix; }



private:

	Mesh* mesh = nullptr;
	Material* material = nullptr;
	Camera* camera = nullptr;
	XMFLOAT3 scale;
	XMFLOAT3 rotation;
	XMFLOAT3 position;

	XMFLOAT4X4 _worldMatrix = {};

};

