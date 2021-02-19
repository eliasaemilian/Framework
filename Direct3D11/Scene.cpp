#include "Scene.h"
#include "Modelmporter.h"
#include "SkyboxMaterial.h"

int Scene::init( ID3D11Device* devIN, ID3D11DeviceContext* devConIN, Camera* cameraIN )
{
	// STORE DEPENDENCIES
	dev = devIN;
	devCon = devConIN;
	camera = cameraIN;

	// INIT LIGHT
	initLight();

	// INIT MESHES
	initMesh( ( char* )"waterplane2.obj" );
	initMesh( ( char* )"Obelisk2.obj" );
	initMesh( ( char* )"inSphere.obj" );

	// INIT MATERIALS
	initMaterial( L"waterTex.jpg", L"VS_WaterShader.hlsl", L"PS_WaterShader.hlsl" );
	initMaterial( L"Obelisk_low_Obelisk_BaseColor.png", L"LightVertexShader.hlsl", L"LightPixelShader.hlsl" );
	initDDSMaterial( L"cubemapSky.DDS", L"VS_Skybox.hlsl", L"PS_Skybox.hlsl" );

	// INIT GAMEOBJECTS
	initGO( 0, XMFLOAT3( 0.0f, -1.5f, 2.0f ), XMFLOAT3( -0.1f, 0.0f, 0.0f ), XMFLOAT3( 1.4f, 1.0f, 1.0f ) );
	initGO( 1, XMFLOAT3( 0.0f, -1.5f, 2.0f ), XMFLOAT3( 0.0f, 0.0f, 0.0f ), XMFLOAT3( 0.08f, 0.08f, 0.08f ) );
	initGO( 2, XMFLOAT3( 0.5f, -1.5f, 2.0f ), XMFLOAT3( 0.0f, 0.0f, 0.0f ), XMFLOAT3( 0.8f, 0.8f, 0.8f ) );

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

void Scene::render(FLOAT time)
{
	// render for all GOS here
	for (int i = 0; i < sceneObjects.size(); i++)
	{
		sceneObjects[i]->render( devCon, time );
	}
}

void Scene::update( FLOAT deltatime )
{
	// update for all GOs here
	for (int i = 0; i < sceneObjects.size(); i++)
	{
		sceneObjects[i]->update( deltatime );
	}
}

void Scene::SetGlobalBuffers()
{
}

void Scene::deInit()
{
	for (int i = 0; i < sceneObjects.size(); i++)
	{
		delete sceneObjects[i];
		meshes[i]->deInit();
		delete meshes[i];
	}
}

void Scene::initLight()
{
	light = new Light();
	light->LightDirection = { 0.0f, 0.0f, 1.0f };
	light->AmbientColor = { 0.9f, 1.0f, 1.0f, 1.0f };
	light->DiffuseColor = { 1.0f, 1.0f, 0.0f, 1.0f };
	light->LightIntensity = 0.5f;
}
