#pragma once
#include "Gameobject.h"
#include "Camera.h"
#include <vector>
#include "PlanarReflection.h"
#include "MirrorMaterial.h"


class Scene
{
public:
	// SCENE INITIALIZATION
	int init( ID3D11Device* dev, ID3D11DeviceContext* devCon, ID3D11DepthStencilView* depthStencilView, FLOAT width, FLOAT height );
	// init system
	void initLight();
	void initCamera( FLOAT width, FLOAT height );

	// init scene objects
	void initGO( int index, bool zWrite, XMFLOAT3 pos, XMFLOAT3 rot, XMFLOAT3 scale, XMFLOAT4 matData_FLOAT[4] );
	void initMesh( char* filenameModel );
	void initMaterialData( bool ZWrite, XMFLOAT4X4 mWorld, XMFLOAT4 f1, XMFLOAT4 f2, XMFLOAT4 f3, XMFLOAT4 f4 );
	void initMaterial( LPCWSTR textureName, LPCWSTR normalMap, LPCWSTR additionalTex, LPCWSTR  vertexShader, LPCWSTR pixelShader );
	void initDDSMaterial( LPCWSTR textureName, LPCWSTR  vertexShader, LPCWSTR pixelShader );

	// SCENE RENDER CALL
	void render( FLOAT time );
	void renderPrecall( FLOAT time );
	void renderZWriteOff( FLOAT time );

	// SCENE UPDATE
	void update( FLOAT deltatime );

	// GPU 
	int createGlobalParamsBuffer( ID3D11Device* pD3DDevice );
	void setGlobalParameters( ID3D11DeviceContext* pD3DDeviceContext, XMFLOAT4X4* view, XMFLOAT4X4* projection, FLOAT time );

	// SCENE DEINITIALIZATION
	void deInit();


private:

	// GLOBAL SHADER PARAMETERS 
	ID3D11Buffer* _globalParamsBuffer = nullptr;
	struct GlobalParameters
	{
		XMFLOAT4X4 CameraViewMatrix;
		XMFLOAT4X4 CameraProjectionMatrix;
		XMFLOAT4 time; // x -> Time.y, y -> deltaTime , zw -> padding
	};

	// REFERENCES
	ID3D11Device* dev;
	ID3D11DeviceContext* devCon;
	ID3D11DepthStencilView* depthStencilView;

	// SCENE SYSTEM 
	Camera* camera;
	Light* light;
	PlanarReflection* pPlanarReflection;

	// SCENE OBJECTS AND COMPONENTS
	std::vector<Gameobject*> sceneObjects = {};
	std::vector<Gameobject*> objectsZWriteOff = {};
	std::vector<Mesh*> meshes = {};
	std::vector<Material*> materials = {};
	std::vector<Material::MaterialBuffer*> materialData = {};
	std::vector<Material::MaterialBuffer*> materialDataZWriteOff = {};

	// OTHER
	FLOAT _waterHeight = -3.5f;
	FLOAT _waterTranslation = 0.0f;
};

