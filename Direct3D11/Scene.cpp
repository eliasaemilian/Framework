#include "Scene.h"
#include "Modelmporter.h"
#include "SkyboxMaterial.h"
#include "PlanarReflection.h"

int Scene::init( ID3D11Device* devIN, ID3D11DeviceContext* devConIN, ID3D11DepthStencilView*  depthStencilViewIN, FLOAT width, FLOAT height )
{
	// STORE DEPENDENCIES
	dev = devIN;
	devCon = devConIN;
	depthStencilView = depthStencilViewIN;

	// INIT GLOBAL SHADER PARAMETER BUFFER
	createGlobalParamsBuffer( dev );

	// INIT CAMERA
	initCamera(width, height);

	// INIT PLANAR REFLECTIONS
	pPlanarReflection = new PlanarReflection();
	pPlanarReflection->init( dev, width, height );


	// INIT LIGHT
	initLight();

	// INIT MESHES
	initMesh( ( char* )"inSphere.obj" );

	initMesh( ( char* )"Obelisk2.obj" );
	initMesh( ( char* )"Sphere.obj" );

	initMesh( ( char* )"meshes/waterplane.obj" );
	//initMesh( ( char* )"waterplane2.obj" );


	// INIT MATERIALS
	initDDSMaterial( L"cubemapSky.DDS", L"VS_Skybox.hlsl", L"PS_Skybox.hlsl" );

	initMaterial( L"Obelisk_low_Obelisk_BaseColor.png", L"Obelisk_low_Obelisk_Normal.png", L"Obelisk_low_Obelisk_Roughness.png", L"VS_Obelisk.hlsl", L"PS_Obelisk.hlsl" );
	initMaterial( L"waterTex.jpg", L"water_normals.jpg", NULL, L"VS_Sphere.hlsl", L"PS_Sphere.hlsl" );

	initMaterial( L"comic_waterTex.png", L"water_normals.jpg", NULL, L"VS_WaterShader.hlsl", L"PS_WaterShader.hlsl" );


	// MATERIAL PARAMS
	XMFLOAT4 mDataSkybox[4] = {};
	XMFLOAT4 mDataObelisk[4] = {};
	XMFLOAT4 mDataSphere[4] = {};
	mDataSphere[0].x = 0.0f;
	mDataSphere[0].y = -2.0f;
	mDataSphere[0].z = 20.0f;
	XMFLOAT4 mDataWater[4] = {};

	mDataObelisk[0].x = -3.5f;


	// INIT GAMEOBJECTS
	// INDEX	| POSITION					| ROTATION						| SCALE
	initGO( 0, TRUE, XMFLOAT3( 0.5f, -1.5f,  0.0f ), XMFLOAT3( 0.0f, 0.0f, 0.0f ), XMFLOAT3( 0.8f, 0.8f, 0.8f ), mDataSkybox );			// [ 0 ] SKYBOX
	initGO( 1, TRUE, XMFLOAT3( 0.0f, -6.0f, 20.0f ), XMFLOAT3( 0.0f, 0.0f, 0.0f ), XMFLOAT3( 0.5f, 0.5f, 0.5f ), mDataObelisk );			// [ 1 ] OBELISK
	initGO( 2, FALSE, XMFLOAT3( 0.0f, -2.0f, 20.0f ), XMFLOAT3( 0.0f, 0.0f, 0.0f ), XMFLOAT3( 1.0f, 1.0f, 1.0f ), mDataSphere );			// [ 2 ] SPHERE
	initGO( 3, TRUE, XMFLOAT3( 0.0f, -3.5f, 10.0f ), XMFLOAT3( -0.01f, 0.0f, 0.0f ), XMFLOAT3( 1.0f, 1.0f, 1.0f ), mDataWater );			// [ 3 ] WATER



	return 0;

}

void Scene::initGO( int index, bool zWrite, XMFLOAT3 pos, XMFLOAT3 rot, XMFLOAT3 scale, XMFLOAT4 MatData_FLOAT[4] )
{
	Gameobject* model = new Gameobject();
	if (zWrite) sceneObjects.push_back( model );
	else objectsZWriteOff.push_back( model );

	model->init( dev, devCon, camera, materials[index], meshes[index] );
	model->SetRotation( rot );
	model->SetScale( scale );
	model->SetPosition( pos );

	// INIT MATERIAL DATA
	initMaterialData( zWrite, *model->getWorldMatrix(), MatData_FLOAT[0], MatData_FLOAT[1], MatData_FLOAT[2], MatData_FLOAT[3] );

}

void Scene::initMesh( char* filenameModel )
{
	Mesh* mesh = new Mesh();
	meshes.push_back( mesh );
	Modelmporter modelimporter;
	modelimporter.init( filenameModel, *mesh );
	mesh->init( dev );

}

void Scene::initMaterialData( bool ZWrite, XMFLOAT4X4 mWorld, XMFLOAT4 f1, XMFLOAT4 f2, XMFLOAT4 f3, XMFLOAT4 f4 )
{
	Material::MaterialBuffer* mBuf = new Material::MaterialBuffer();
	if (ZWrite)materialData.push_back( mBuf );
	else materialDataZWriteOff.push_back( mBuf );

	mBuf->WORLD_MATRIX = mWorld;


	mBuf->PARAM_FLOAT4_1 = f1;
	mBuf->PARAM_FLOAT4_2 = f2;
	mBuf->PARAM_FLOAT4_3 = f3;
	mBuf->PARAM_FLOAT4_4 = f4;
}

void Scene::initMaterial( LPCWSTR textureName, LPCWSTR normalMap, LPCWSTR additionalTex, LPCWSTR  vertexShader, LPCWSTR pixelShader )
{
	Material* mat = new Material();
	materials.push_back( mat );

	mat->init( dev, textureName, normalMap, additionalTex, vertexShader, pixelShader );
	mat->setLight( devCon, *light );
}

void Scene::initDDSMaterial( LPCWSTR textureName, LPCWSTR vertexShader, LPCWSTR pixelShader )
{
	Material* mat = new SkyboxMaterial();
	materials.push_back( mat );

	mat->init( dev, textureName, NULL, NULL, vertexShader, pixelShader );
	mat->setLight( devCon, *light );
}





void Scene::render( FLOAT time )
{
	// SET GLOBAL PARAMETERS
	setGlobalParameters( devCon, camera->getViewMatrix(), camera->getProjectionMatrix(), time );

	// SET REFLECTION MATRIX AND TEXTURE
	pPlanarReflection->setPlanarReflection( devCon, camera->getReflectionMatrix() );

	// RENDER ALL SCENE OBJECTS
	for (int i = 0; i < sceneObjects.size(); i++)
	{
		sceneObjects[i]->render( devCon, materialData[i] );
	}

	//sceneObjects[0]->render( devCon, materialData[0] );


}

void Scene::renderZWriteOff( FLOAT time )
{
	for (int i = 0; i < objectsZWriteOff.size(); i++)
	{
		objectsZWriteOff[i]->render( devCon, materialDataZWriteOff[i] );
	}


}

void Scene::renderPrecall( FLOAT time )
{
	// PREPARE REFLECTION RENDER TARGET FOR DRAWCALL
	pPlanarReflection->setRenderTargetToTexture( devCon, depthStencilView );
	pPlanarReflection->clearRenderTargetView( devCon );


	// RENDER SCENE OBJECTS	TO TEXTURE WITH REFLECTION VIEW MATRIX
	camera->RenderReflection( -3.5f );
	setGlobalParameters( devCon, camera->getReflectionMatrix(), camera->getProjectionMatrix(), time );
	for (int i = 0; i < sceneObjects.size() - 1; i++)
	{
		sceneObjects[i]->render( devCon, materialData[i] );
	}


}

void Scene::update( FLOAT deltatime )
{
	for (int i = 0; i < sceneObjects.size(); i++)
	{
		sceneObjects[i]->update( deltatime );
	}
}

int Scene::createGlobalParamsBuffer( ID3D11Device* pD3DDevice )
{
	D3D11_BUFFER_DESC desc = {};
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.ByteWidth = sizeof( GlobalParameters );
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	HRESULT hr = pD3DDevice->CreateBuffer( &desc, nullptr, &_globalParamsBuffer );
	if (FAILED( hr )) return 46;

	return 0;
}


void Scene::initLight()
{
	light = new Light();
	light->LightDirection = { 0.3f, 0.7f, 0.0f };
	light->AmbientColor = { 0.9f, 1.0f, 1.0f, 1.0f };
	light->DiffuseColor = { 0.0f, 0.0f, 0.0f, 1.0f };
	light->LightIntensity = 2.0f;

}

void Scene::initCamera( FLOAT width, FLOAT height )
{
	XMFLOAT3 camPos = { 0.0f, 0.0f, -1.0f };

	camera = new Camera();
	camera->init( width, height, camPos );
}

void Scene::deInit()
{
	for (int i = 0; i < sceneObjects.size(); i++)
	{
		delete sceneObjects[i];
		meshes[i]->deInit();
		delete meshes[i];
		materials[i]->deInit();
		delete materials[i];
	}

	delete light;

	camera->deInit();
	delete camera;

	pPlanarReflection->deinit();
	delete pPlanarReflection;
}

void Scene::setGlobalParameters( ID3D11DeviceContext* pD3DDeviceContext, XMFLOAT4X4* view, XMFLOAT4X4* projection, FLOAT time )
{
	D3D11_MAPPED_SUBRESOURCE data = {};
	HRESULT hr = pD3DDeviceContext->Map( _globalParamsBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &data );
	if (FAILED( hr )) return;

	XMMATRIX viewMatrix = XMLoadFloat4x4( view );
	XMMATRIX projectionMatrix = XMLoadFloat4x4( projection );


	XMVECTOR t = XMVectorSet( time, time, time, time );

	viewMatrix = XMMatrixTranspose( viewMatrix );
	projectionMatrix = XMMatrixTranspose( projectionMatrix );

	GlobalParameters* pBuffer = reinterpret_cast< GlobalParameters* >( data.pData );
	XMStoreFloat4x4( &pBuffer->CameraViewMatrix, viewMatrix );
	XMStoreFloat4x4( &pBuffer->CameraProjectionMatrix, projectionMatrix );
	XMStoreFloat4( &pBuffer->time, t );

	pD3DDeviceContext->Unmap( _globalParamsBuffer, 0 );

	pD3DDeviceContext->VSSetConstantBuffers( 0, 1, &_globalParamsBuffer );
}
