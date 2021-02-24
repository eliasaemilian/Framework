#pragma once
#include "Scene.h"
#include "Modelmporter.h"
#include "SkyboxMaterial.h"
#include "PlanarReflection.h"
#include "Utils.h"

int Scene::init( ID3D11Device* devIN, ID3D11DeviceContext* devConIN, ID3D11DepthStencilView*  depthStencilViewIN, INT width, INT height )
{
	int err;

	// STORE DEPENDENCIES
	_pDev = devIN;
	_pDevCon = devConIN;
	_pDepthStencilView = depthStencilViewIN;

	// SET WATER HEIGHT
	_waterHeight = -3.5f;

	// INIT GLOBAL SHADER PARAMETER BUFFER
	createGlobalParamsBuffer();

	// INIT CAMERA
	XMFLOAT3 camPos = { 0.0f, 1.5f, -1.0f };	
	err = initCamera( width, height, camPos );
	if (err != 0) return err;

	// INIT PLANAR REFLECTIONS
	_pPlanarReflection = new PlanarReflection();
	_pPlanarReflection->init( _pDev, width, height );


	// INIT LIGHT
	err = initLight();
	if (err != 0) return err;

	// INIT TIME
	err = initTime();
	if (err != 0) return err;

	// INIT MESHES
	initMesh( ( char* )"meshes/inSphere.obj" );
	initMesh( ( char* )"meshes/Obelisk2.obj" );
	initMesh( ( char* )"meshes/dome.obj" );
	initMesh( ( char* )"meshes/waterplane.obj" );

	// INIT MATERIALS
	initDDSMaterial( L"textures/cubemap_otherworld.DDS", L"VS_Skybox.hlsl", L"PS_Skybox.hlsl" );
	initMaterial( L"Obelisk_low_Obelisk_BaseColor.png", L"Obelisk_low_Obelisk_Normal.png", L"Obelisk_low_Obelisk_Roughness.png", L"VS_Obelisk.hlsl", L"PS_Obelisk.hlsl" );
	initMaterial( NULL, NULL, NULL, L"VS_Sphere.hlsl", L"PS_Sphere.hlsl" );
	initMaterial( L"waterTex.jpg", L"water_normals.jpg", NULL, L"VS_WaterShader.hlsl", L"PS_WaterShader.hlsl" );


	// SET MATERIAL PARAMS
	XMFLOAT4 mDataSkybox[4] = {};
	XMFLOAT4 mDataObelisk[4] = {};
	XMFLOAT4 mDataSphere[4] = {};
	mDataSphere[0].x = 0.0f;
	mDataSphere[0].y = -2.0f;
	mDataSphere[0].z = 20.0f;
	XMFLOAT4 mDataWater[4] = {};

	mDataObelisk[0] = XMFLOAT4( _pCamera->getCameraPos()->x, _pCamera->getCameraPos()->y, _pCamera->getCameraPos()->z, _waterHeight );
	mDataObelisk[1] = XMFLOAT4( 0.1f, 5.8f, 0.9f, 6.0f ); // LIGHTING X [ AMBIENT INTENSITY ], Y [ DIFFUSE INTENSITY ], Z [ SPECULAR POWER ], W [ MODEL NORMAL STRENGTH ]
	mDataWater[1] = XMFLOAT4( 0.1f, 4.0f, 0.2f, 0.0f );  // X [ REFRACTION SCALE ], Y = [ REFRACTION TEXTURE TILING ], Z = [ REFRACTION SPEED]


	// INIT GAMEOBJECTS
	// INDEX	| POSITION					| ROTATION						| SCALE
	initGO( 0, TRUE, XMFLOAT3( 0.5f, 0.0f,  0.0f ), XMFLOAT3( 0.0f, 0.0f, 0.0f ), XMFLOAT3( 0.8f, 0.8f, 0.8f ), mDataSkybox );			// [ 0 ] SKYBOX
	initGO( 1, TRUE, XMFLOAT3( 0.0f, -6.0f, 20.0f ), XMFLOAT3( 0.0f, 1.9f, 0.0f ), XMFLOAT3( 0.5f, 0.5f, 0.5f ), mDataObelisk );			// [ 1 ] OBELISK
	initGO( 2, FALSE, XMFLOAT3( 0.0f, -2.0f, 20.0f ), XMFLOAT3( 0.0f, 0.0f, 0.0f ), XMFLOAT3( 1.11f, 1.11f, 1.11f ), mDataSphere );			// [ 2 ] SPHERE
	initGO( 3, TRUE, XMFLOAT3( 0.0f, _waterHeight, 10.0f ), XMFLOAT3( -0.01f, 0.0f, 0.0f ), XMFLOAT3( 1.0f, 1.0f, 1.0f ), mDataWater );			// [ 3 ] WATER



	return 0;

}


// ----------------------------------------- INITIALIZATION ----------------------------------------------- //

int Scene::initLight()
{
	_pLight = new Light();
	_pLight->LightDirection = { 0.3f, 0.5f, 0.0f };
	_pLight->AmbientColor = { 0.9f, 1.0f, 1.0f, 1.0f };
	_pLight->DiffuseColor = { 0.98f, 0.867f, 0.945f, 1.0f };
	_pLight->LightIntensity = 1.0f;

	return 0;
}

int Scene::initTime()
{
	_pTime = new Time();
	return _pTime->init();
}

int Scene::initCamera( INT width, INT height, XMFLOAT3 camPos )
{
	_pCamera = new Camera();
	return _pCamera->init( width, height, camPos );
}

void Scene::initGO( int index, bool zWrite, XMFLOAT3 pos, XMFLOAT3 rot, XMFLOAT3 scale, XMFLOAT4 MatData_FLOAT[4] )
{
	Gameobject* model = new Gameobject();
	if (zWrite) _pSceneObjects.push_back( model );
	else _pObjectsZWriteOff.push_back( model );

	model->init( _pDev, _pDevCon, _pCamera, _pMaterials[index], _pMeshes[index] );
	model->SetRotation( rot );
	model->SetScale( scale );
	model->SetPosition( pos );

	// INIT MATERIAL DATA
	initMaterialData( zWrite, *model->getWorldMatrix(), MatData_FLOAT[0], MatData_FLOAT[1], MatData_FLOAT[2], MatData_FLOAT[3] );

}

void Scene::initMesh( char* filenameModel )
{
	Mesh* mesh = new Mesh();
	_pMeshes.push_back( mesh );
	Modelmporter modelimporter;
	modelimporter.init( filenameModel, *mesh );
	mesh->init( _pDev );

}

void Scene::initMaterialData( bool ZWrite, XMFLOAT4X4 mWorld, XMFLOAT4 f1, XMFLOAT4 f2, XMFLOAT4 f3, XMFLOAT4 f4 )
{
	Material::MaterialBuffer* mBuf = new Material::MaterialBuffer();
	if (ZWrite)_pMaterialData.push_back( mBuf );
	else _pMaterialDataZWriteOff.push_back( mBuf );

	mBuf->WORLD_MATRIX = mWorld;


	mBuf->PARAM_FLOAT4_1 = f1;
	mBuf->PARAM_FLOAT4_2 = f2;
	mBuf->PARAM_FLOAT4_3 = f3;
	mBuf->PARAM_FLOAT4_4 = f4;
}

void Scene::initMaterial( LPCWSTR textureName, LPCWSTR normalMap, LPCWSTR additionalTex, LPCWSTR  vertexShader, LPCWSTR pixelShader )
{
	Material* mat = new Material();
	_pMaterials.push_back( mat );

	mat->init( _pDev, textureName, normalMap, additionalTex, vertexShader, pixelShader );
	mat->setLight( _pDevCon, *_pLight );
}

void Scene::initDDSMaterial( LPCWSTR textureName, LPCWSTR vertexShader, LPCWSTR pixelShader )
{
	Material* mat = new SkyboxMaterial();
	_pMaterials.push_back( mat );

	mat->init( _pDev, textureName, NULL, NULL, vertexShader, pixelShader );
	mat->setLight( _pDevCon, *_pLight );
}


// ----------------------------------------- UPDATE ----------------------------------------------- //

float degree = 0;
bool countingDown = false; float procentual = 0.0f;
void Scene::update()
{
	// UPDATE WATER PLANE

	// Update the position of the water to simulate motion.
	if (_waterTranslation > 10.0f) countingDown = true;
	else if (_waterTranslation <= 0.001f) countingDown = false;

	if (!countingDown) _waterTranslation += 0.001f;
	else _waterTranslation -= 0.001f;

	// set updated Water Translation to WaterMatBuffer
	_pMaterialData[2]->PARAM_FLOAT4_1.x = _waterTranslation;

	// UPDATE TIMELAPS PARAM
	float daynightborder = .75;
	// update light for timelapse with smooth overlap from long sunset to fast runrise in accordance with cubemap
	float lightDirY = 0.0f; float lightDirX = 0.0f;  float scaleAdjust = 1.0f;
	float final = 1.1f; float start = 1.2f;

	if (_timelapseCounter >= 720 && procentual > 1)
	{
		_timelapseCounter = 0;
		_pLight->LightIntensity = 1;
		lightDirY = 1;
		lightDirX = 0.4f;
		scaleAdjust = final;
		procentual = 0;
	}
	else
	{
		_timelapseCounter += 0.1f; // 0.1

		procentual = _timelapseCounter / 720;
		float procentualDay = procentual / 0.75f;

		if (procentual <= daynightborder)
		{
			_pLight->LightIntensity = 1 - procentual;
			lightDirY = 1 - procentual;
			lightDirX = linLerp( 0.4f, 0, procentualDay );
			scaleAdjust = linLerp( final, start, procentualDay );

		}

		else
		{
			float pSunrise = static_cast<FLOAT>( ( ( 1 - (double)procentual ) / (1 - (double)daynightborder ) - 1 ) * -1 );
			_pLight->LightIntensity = 1 - daynightborder + pSunrise;
			if (_pLight->LightIntensity > 1) _pLight->LightIntensity = 1;

			lightDirY = _pLight->LightIntensity;
			lightDirX = linLerp( 0, 0.4f, pSunrise );
			scaleAdjust = linLerp( start, final, pSunrise );

		}
	}
	

	// adjust sphere scale
	_pObjectsZWriteOff[0]->SetScale( XMFLOAT3( scaleAdjust, scaleAdjust, scaleAdjust ));
	_pMaterialDataZWriteOff[0]->WORLD_MATRIX = *_pObjectsZWriteOff[0]->getWorldMatrix();

	_pLight->LightDirection = { lightDirX, lightDirY, 0.0f };
	_pLight->AmbientColor = { 0.9f, 1.0f, 1.0f, 1.0f };
	_pLight->DiffuseColor = { 0.98f, 0.867f, 0.945f, 1.0f };


	_pMaterialData[0]->PARAM_FLOAT4_1.x = _timelapseCounter;

	// CALL UPDATE FOR ALL OBJECTS IN SCENE
	for (int i = 0; i < _pSceneObjects.size(); i++)
	{
		_pSceneObjects[i]->update( _pTime->getDeltaTime() );
		_pMaterials[i] ->setLight( _pDevCon, *_pLight );
	}
}


// ----------------------------------------- RENDER ----------------------------------------------- //

void Scene::ReflectionsRenderpass()
{
	// PREPARE REFLECTION RENDER TARGET FOR DRAWCALL
	_pPlanarReflection->setRenderTargetToTexture( _pDevCon, _pDepthStencilView );
	_pPlanarReflection->clearRenderTargetView( _pDevCon );

	// SET CUTOFF TO WATER HEIGHT
	_pMaterialData[1]->PARAM_FLOAT4_1.w = _waterHeight;

	// RENDER SCENE OBJECTS	TO TEXTURE WITH REFLECTION VIEW MATRIX
	_pCamera->RenderReflection( -3.5f );
	setGlobalParameters( _pCamera->getReflectionMatrix(), _pCamera->getProjectionMatrix(), _pTime->getShaderTimeParam() );
	for (int i = 0; i < _pSceneObjects.size() - 1; i++)
	{
		_pSceneObjects[i]->render( _pDevCon, _pMaterialData[i] );
	}
}

void Scene::GeometryRenderpass()
{
	// SET GLOBAL PARAMETERS
	setGlobalParameters( _pCamera->getViewMatrix(), _pCamera->getProjectionMatrix(), _pTime->getShaderTimeParam() );

	// SET REFLECTION MATRIX AND TEXTURE
	_pPlanarReflection->setPlanarReflection( _pDevCon, _pCamera->getReflectionMatrix() );

	// UPDATE WATER Y FOE OBELISK TO ENABLE RENDERING BELOW WATER SURFACE
	_pMaterialData[1]->PARAM_FLOAT4_1.w = -100;

	// RENDER ALL SCENE OBJECTS
	for (int i = 0; i < _pSceneObjects.size(); i++)
	{
		_pSceneObjects[i]->render( _pDevCon, _pMaterialData[i] );
	}
}

void Scene::ZWriteOffRenderpass()
{
	for (int i = 0; i < _pObjectsZWriteOff.size(); i++)
	{
		_pObjectsZWriteOff[i]->render( _pDevCon, _pMaterialDataZWriteOff[i] );
	}
}


// ----------------------------------------- GLOBAL SHADER PARAMS BUFFER ----------------------------------------------- //

int Scene::createGlobalParamsBuffer()
{
	D3D11_BUFFER_DESC desc = {};
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.ByteWidth = sizeof( GlobalParameters );
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	HRESULT hr = _pDev->CreateBuffer( &desc, nullptr, &_pGlobalParamsBuffer );
	if (FAILED( hr )) return 46;

	return 0;
}
void Scene::setGlobalParameters( XMFLOAT4X4* view, XMFLOAT4X4* projection, XMFLOAT4* t )
{
	D3D11_MAPPED_SUBRESOURCE data = {};
	HRESULT hr = _pDevCon->Map( _pGlobalParamsBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &data );
	if (FAILED( hr )) return;

	XMMATRIX viewMatrix = XMLoadFloat4x4( view );
	XMMATRIX projectionMatrix = XMLoadFloat4x4( projection );

	viewMatrix = XMMatrixTranspose( viewMatrix );
	projectionMatrix = XMMatrixTranspose( projectionMatrix );

	XMVECTOR tVec = XMLoadFloat4( t );

	GlobalParameters* pBuffer = reinterpret_cast< GlobalParameters* >( data.pData );
	XMStoreFloat4x4( &pBuffer->CameraViewMatrix, viewMatrix );
	XMStoreFloat4x4( &pBuffer->CameraProjectionMatrix, projectionMatrix );
	XMStoreFloat4( &pBuffer->time, tVec );

	_pDevCon->Unmap( _pGlobalParamsBuffer, 0 );

	_pDevCon->VSSetConstantBuffers( 0, 1, &_pGlobalParamsBuffer );
}


// ----------------------------------------- DEINITIALIZATION ----------------------------------------------- //

void Scene::deInit()
{
	for (int i = 0; i < _pSceneObjects.size(); i++)
	{
		delete _pSceneObjects[i];
		_pMeshes[i]->deInit();
		delete _pMeshes[i];
		_pMaterials[i]->deInit();
		delete _pMaterials[i];
	}

	delete _pLight;

	_pTime->deInit();
	delete _pTime;

	_pCamera->deInit();
	delete _pCamera;

	_pPlanarReflection->deinit();
	delete _pPlanarReflection;
}

// LINEAR INTERPOLATION BETWEEN TWO VALUES
float Scene::linLerp( float a, float b, float f )
{
	return static_cast< FLOAT >( ( a * ( 1.0f - f ) ) + ( b * ( double )f ) );
}
