#pragma once
#include "Gameobject.h"
#include "Camera.h"
#include <vector>
#include "PlanarReflection.h"
#include "MirrorMaterial.h"


class Scene
{
public:
	int init( ID3D11Device* dev, ID3D11DeviceContext* devCon, ID3D11DepthStencilView* depthStencilView, FLOAT width, FLOAT height );
	void deInit();
	void initLight();
	void initCamera( FLOAT width, FLOAT height );
	void initGO( int index, XMFLOAT3 pos, XMFLOAT3 rot, XMFLOAT3 scale, XMFLOAT4 matData_FLOAT[4] );
	void initMesh( char* filenameModel );
	void initMaterialData( XMFLOAT4X4 mWorld, XMFLOAT4 f1, XMFLOAT4 f2, XMFLOAT4 f3, XMFLOAT4 f4 );
	void initMaterial( LPCWSTR textureName, LPCWSTR  vertexShader, LPCWSTR pixelShader );
	void initDDSMaterial( LPCWSTR textureName, LPCWSTR  vertexShader, LPCWSTR pixelShader );
	void initPlanarReflection( UINT width, UINT height );
	void setPlanarReflection();
	bool initRTexture( UINT width, UINT height );
	void initMirrorMaterial( LPCWSTR textureName, LPCWSTR  vertexShader, LPCWSTR pixelShader );
	void render( FLOAT time );
	void renderPrecall( FLOAT time );
	void update( FLOAT deltatime );
	int createGlobalParamsBuffer( ID3D11Device* pD3DDevice );
	void setGlobalParameters( ID3D11DeviceContext* pD3DDeviceContext, XMFLOAT4X4* view, XMFLOAT4X4* projection, FLOAT time );

	void setPlanarReflectionTexture( );

	std::vector<Gameobject*> sceneObjects = {};
	std::vector<Mesh*> meshes = {};
	std::vector<Material*> materials = {};
	std::vector<Material::MaterialBuffer*> materialData = {};
private:

	ID3D11Buffer* _globalParamsBuffer = nullptr;
	struct GlobalParameters
	{
		XMFLOAT4X4 CameraViewMatrix;
		XMFLOAT4X4 CameraProjectionMatrix;
		XMFLOAT4 time; // x -> Time.y, y -> deltaTime , zw -> padding
	};

	ID3D11Buffer* _pReflectionBuffer = nullptr;
	struct ReflectionBuffer
	{
		XMFLOAT4X4 reflectionMatrix;
	};


	ID3D11Texture2D* m_renderTargetTexture;
	ID3D11RenderTargetView* m_renderTargetView;
	ID3D11ShaderResourceView* m_shaderResourceView;

	PlanarReflection* pPlanarReflection;
	MirrorMaterial* pMirrorMaterial;

	ID3D11Device* dev;
	ID3D11DeviceContext* devCon;
	ID3D11DepthStencilView* depthStencilView;

	Camera* camera;
	Light* light;
};

