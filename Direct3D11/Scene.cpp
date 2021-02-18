#include "Scene.h"
#include "Modelmporter.h"

void Scene::init( ID3D11Device* dev, ID3D11DeviceContext* devCon, Camera* camera )
{

}

void Scene::initGO( ID3D11Device* dev, ID3D11DeviceContext* devCon, Camera* camera, char* filenameModel, Material* material, XMFLOAT3 pos, XMFLOAT3 rot, XMFLOAT3 scale )
{
	Gameobject model = {};
	Mesh mesh;
	Modelmporter modelimporter;
	modelimporter.init( filenameModel, mesh );
	mesh.init( dev );
	model.init( dev, devCon, camera, material, &mesh );
	model.SetPosition( pos );
	model.SetScale( scale );
	model.SetRotation( rot );
	
	sceneObjects.push_back( model );
}

void Scene::render()
{
	// render for all GOS here
}

void Scene::update()
{
	// update for all GOs here
}
