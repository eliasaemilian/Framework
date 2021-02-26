#pragma once
#include "Gameobject.h"
#include "Camera.h"
#include <vector>
#include "PlanarReflection.h"
#include "MirrorMaterial.h"
#include "Time.h"

class Scene
{
public:
	// SCENE INITIALIZATION
	int init( ID3D11Device* dev, ID3D11DeviceContext* devCon, ID3D11DepthStencilView* depthStencilView, INT width, INT height );

	// SCENE UPDATE
	void update();

	// SCENE RENDER CALL
	void ReflectionsRenderpass();
	void GeometryRenderpass();
	void ZWriteOffRenderpass();

	// SCENE DEINITIALIZATION
	void deInit();


private:

	// init system
	int initLight();
	int initTime();
	int initCamera( INT width, INT height, XMFLOAT3 camPos );

	// init scene objects
	void initGO( int index, bool zWrite, XMFLOAT3 pos, XMFLOAT3 rot, XMFLOAT3 scale, XMFLOAT4 matData_FLOAT[4] );
	void initMesh( char* filenameModel );
	void initMaterialData( bool ZWrite, XMFLOAT4X4 mWorld, XMFLOAT4 f1, XMFLOAT4 f2, XMFLOAT4 f3, XMFLOAT4 f4 );
	void initMaterial( LPCWSTR textureName, LPCWSTR normalMap, LPCWSTR additionalTex, LPCWSTR  vertexShader, LPCWSTR pixelShader );
	void initDDSMaterial( LPCWSTR textureName, LPCWSTR  vertexShader, LPCWSTR pixelShader );

	// GLOBAL SHADER PARAMS AND LIGHTDATA
	int createGlobalParamsBuffer();
	void setGlobalParameters( XMFLOAT4X4* view, XMFLOAT4X4* projection, XMFLOAT4* t );
	int createLightDataBuffer();
	void setLightDataBuffer();

	// HELPER
	float linLerp( float a, float b, float f );

private:

	// GLOBAL SHADER PARAMETERS [ VS BUFFER REGISTER 0]
	ID3D11Buffer* _pGlobalParamsBuffer = nullptr;
	struct GlobalParameters
	{
		XMFLOAT4X4 CameraViewMatrix;
		XMFLOAT4X4 CameraProjectionMatrix;
		XMFLOAT4 time; 
	};

	// GLOBAL LIGHTDATA [PS BUFFER REGISTER 0 ]
	ID3D11Buffer* _pLightDataBuffer = nullptr;
	struct LightDataBuffer
	{
		Light lightData;
	};

	// REFERENCES
	ID3D11Device* _pDev;
	ID3D11DeviceContext* _pDevCon;
	ID3D11DepthStencilView* _pDepthStencilView;

	// SCENE SYSTEM 
	Camera* _pCamera;
	Light* _pLight;
	Time* _pTime;
	PlanarReflection* _pPlanarReflection;

	// SCENE OBJECTS AND COMPONENTS
	std::vector<Gameobject*> _pRenderqueueOpaque = {};
	std::vector<Gameobject*> pRenderqueueTransparent = {};
	std::vector<Mesh*> _pMeshes = {};
	std::vector<Material*> _pMaterials = {};
	std::vector<Material::MaterialBuffer*> _pMaterialData = {};
	std::vector<Material::MaterialBuffer*> _pMaterialDataZWriteOff = {};

	// WATER AND TIMELAPSE PARAMS
	FLOAT _waterHeight = -3.5f;
	FLOAT _waterTranslation = 0.0f;
	FLOAT _timelapseCounter = 0.0f;
};

