#pragma once
#include "Gameobject.h"
#include "Camera.h"
#include <vector>

class Scene
{
public:
	int init( ID3D11Device* dev, ID3D11DeviceContext* devCon, Camera* cam );
	void deInit();
	void initLight();
	void initGO( int index, XMFLOAT3 pos, XMFLOAT3 rot, XMFLOAT3 scale );
	void initMesh( char* filenameModel );
	void initMaterial( LPCWSTR textureName, LPCWSTR  vertexShader, LPCWSTR pixelShader );
	void initDDSMaterial( LPCWSTR textureName, LPCWSTR  vertexShader, LPCWSTR pixelShader );
	void render( FLOAT time );
	void update( FLOAT deltatime );
	void SetGlobalBuffers();

	std::vector<Gameobject*> sceneObjects = {};
	std::vector<Mesh*> meshes = {};
	std::vector<Material*> materials = {};
private:

	ID3D11Device* dev;
	ID3D11DeviceContext* devCon;
	Camera* camera;
	Light* light;
};

