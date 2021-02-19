#include "Scene.h"
#include "Modelmporter.h"
#include "SkyboxMaterial.h"

int Scene::init( ID3D11Device* devIN, ID3D11DeviceContext* devConIN, FLOAT width, FLOAT height )
{
	// STORE DEPENDENCIES
	dev = devIN;
	devCon = devConIN;

	// INIT CAMERA
	initCamera(width, height);

	// INIT LIGHT
	initLight();

	// INIT MESHES
	initMesh( ( char* )"waterplane2.obj" );
	initMesh( ( char* )"Obelisk2.obj" );
	initMesh( ( char* )"inSphere.obj" );

	// INIT MATERIALS
	initMaterial( L"water_normals.jpg", L"VS_WaterShader.hlsl", L"PS_WaterShader.hlsl" );
	initMaterial( L"Obelisk_low_Obelisk_BaseColor.png", L"LightVertexShader.hlsl", L"LightPixelShader.hlsl" );
	initDDSMaterial( L"cubemapSky.DDS", L"VS_Skybox.hlsl", L"PS_Skybox.hlsl" );

	// INIT GAMEOBJECTS
	// INDEX	| POSITION					| ROTATION						| SCALE
	initGO( 0, XMFLOAT3( 0.0f, -3.5f, 2.0f ), XMFLOAT3( -0.1f, 0.0f, 0.0f ), XMFLOAT3( 1.0f, 1.0f, 1.0f ) );	// [ 0 ] WATER
	initGO( 1, XMFLOAT3( 0.0f, -12.0f, 40.0f ), XMFLOAT3( 0.0f, 0.0f, 0.0f ), XMFLOAT3( 1.0f, 1.0f, 1.0f ) );		// [ 1 ] OBELISK
	initGO( 2, XMFLOAT3( 0.5f, -1.5f, 2.0f ), XMFLOAT3( 0.0f, 0.0f, 0.0f ), XMFLOAT3( 0.8f, 0.8f, 0.8f ) );		// [ 2 ] SKYBOX

	return 0;

}

void Scene::initGO( int index, XMFLOAT3 pos, XMFLOAT3 rot, XMFLOAT3 scale )
{
	Gameobject* model = new Gameobject();
	sceneObjects.push_back( model );
	model->init( dev, devCon, camera, materials[index], meshes[index] );
	model->SetRotation( rot );
	model->SetScale( scale );
	model->SetPosition( pos );
}

void Scene::initMesh( char* filenameModel )
{
	Mesh* mesh = new Mesh();
	meshes.push_back( mesh );
	Modelmporter modelimporter;
	modelimporter.init( filenameModel, *mesh );
	mesh->init( dev );

}

void Scene::initMaterial( LPCWSTR textureName, LPCWSTR  vertexShader, LPCWSTR pixelShader )
{
	Material* mat = new Material();
	materials.push_back( mat );

	mat->init( dev, textureName, vertexShader, pixelShader );
	mat->setLight( devCon, *light );
}

void Scene::initDDSMaterial( LPCWSTR textureName, LPCWSTR vertexShader, LPCWSTR pixelShader )
{
	Material* mat = new SkyboxMaterial();
	materials.push_back( mat );

	mat->init( dev, textureName, vertexShader, pixelShader );
	mat->setLight( devCon, *light );
}

void Scene::render( FLOAT time )
{
	for (int i = 0; i < sceneObjects.size(); i++)
	{
		sceneObjects[i]->render( devCon, time );
	}
}

void Scene::update( FLOAT deltatime )
{
	for (int i = 0; i < sceneObjects.size(); i++)
	{
		sceneObjects[i]->update( deltatime );
	}
}

void Scene::SetGlobalBuffers()
{
	// TODO: SET GLOBAL SHADER PARAMS LIKE TIME, MATRIXBUFFERS HERE
}

void Scene::initLight()
{
	light = new Light();
	light->LightDirection = { 0.0f, 1.0f, 0.0f };
	light->AmbientColor = { 0.9f, 1.0f, 1.0f, 1.0f };
	light->DiffuseColor = { 0.0f, 0.0f, 0.0f, 1.0f };
	light->LightIntensity = 0.5f;
}

void Scene::initCamera( FLOAT width, FLOAT height )
{
	camera = new Camera();
	camera->init( width, height );
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
}
