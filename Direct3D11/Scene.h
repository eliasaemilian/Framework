#pragma once
#include "Gameobject.h"
#include "Camera.h"
#include <vector>


class Scene
{
public:
	int init( ID3D11Device* dev, ID3D11DeviceContext* devCon, FLOAT width, FLOAT height );
	void deInit();
	void initLight();
	void initCamera( FLOAT width, FLOAT height );
	void initGO( int index, XMFLOAT3 pos, XMFLOAT3 rot, XMFLOAT3 scale );
	void initMesh( char* filenameModel );
	void initMaterialData( XMFLOAT4X4 m1, XMFLOAT4X4 m2, XMFLOAT4X4 m3, XMFLOAT4X4 m4, XMFLOAT4 f1, XMFLOAT4 f2, XMFLOAT4 f3, XMFLOAT4 f4 );
	void initMaterial( LPCWSTR textureName, LPCWSTR  vertexShader, LPCWSTR pixelShader );
	void initDDSMaterial( LPCWSTR textureName, LPCWSTR  vertexShader, LPCWSTR pixelShader );
	void render( FLOAT time );
	void update( FLOAT deltatime );
	void SetGlobalBuffers();

	std::vector<Gameobject*> sceneObjects = {};
	std::vector<Mesh*> meshes = {};
	std::vector<Material*> materials = {};
	std::vector<Material::MaterialBuffer*> materialData = {};
private:

	ID3D11Device* dev;
	ID3D11DeviceContext* devCon;
	Camera* camera;
	Light* light;
};

