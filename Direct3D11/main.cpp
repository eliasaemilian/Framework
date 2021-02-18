#pragma once
#include <Windows.h>
#include "Window.h"
#include "D3D.h"
#include "Camera.h"
#include "Time.h"
#include "Material.h"
#include "Light.h"
#include "Gameobject.h"
#include "Modelmporter.h"
#include "Scene.h"
//#include "Skybox.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR szCmdLine, int nCmdShow)
{
	INT width = 800;
	INT height = 600;
	bool isWindowed = true;

	// 1. create window
	Window window;
	int error = window.init(hInstance, width, height, nCmdShow);
	if (error != 0) return error;

	// 2. create Direct3D 11 interface
	D3D d3d;
	error = d3d.init(window.getWindowHandle(), width, height, isWindowed);
	if (error != 0) return error;

	// 3. create mesh
	//Mesh mesh;
	//error = mesh.init(d3d.getDevice());
	//if (error != 0) return error;

	// 4. create camera
	Camera camera;
	error = camera.init(width, height);
	if (error != 0) return error;

	// 5. create time
	Time time;
	error = time.init();
	if (error != 0) return error;

	// plane
	Material material;
	error = material.init( d3d.getDevice(), L"waterTex.jpg", L"LightVertexShader.hlsl", L"LightPixelShader.hlsl" );
	if (error != 0) return error;

	// 7. create light
	Light light = {};
	light.LightDirection = { 0.0f, 0.0f, 1.0f };
	light.AmbientColor = { 0.9f, 1.0f, 1.0f, 1.0f };
	light.DiffuseColor = { 1.0f, 1.0f, 0.0f, 1.0f };
	light.LightIntensity = 0.5f;
	material.setLight(d3d.getDeviceContext(), light);

	// Create Gameobject
	

	


	
	

	Material waterMat;
	waterMat.init( d3d.getDevice(), L"waterTex.jpg", L"LightVertexShader.hlsl", L"LightPixelShader.hlsl" );



	//// SKYBOX
	//Skybox skybox;
	// skybox.init();


	//// MODEL IMPORT
	Gameobject model = {};
	Mesh iMesh;
	int vCount;
	Modelmporter modelimporter;
	modelimporter.init( (char*)"waterplane.obj", iMesh);
	iMesh.init( d3d.getDevice() );
	model.init( d3d.getDevice(), d3d.getDeviceContext(), &camera, &waterMat, &iMesh );
	model.SetPosition( XMFLOAT3( 0.0f, -1.5f, 2.0f ) );
	//model.SetScale( XMFLOAT3( 0.05f, 0.05f, 0.05f ) );
	model.SetScale( XMFLOAT3( 1.4f, 1.0f, 1.0f ) );
	model.SetRotation( XMFLOAT3( -0.1f, 0.0f, 0.0f ) );

	// SCENE

	Scene scene = {};
	scene.initGO( d3d.getDevice(), d3d.getDeviceContext(), &camera, ( char* )"waterplane.obj", &waterMat, XMFLOAT3( 0.0f, -1.5f, 2.0f ), XMFLOAT3( -0.1f, 0.0f, 0.0f ), XMFLOAT3( 1.4f, 1.0f, 1.0f ) );



	// OBELISK
	Material obMat;
	obMat.init( d3d.getDevice(), L"Obelisk_low_Obelisk_BaseColor.png", L"LightVertexShader.hlsl", L"LightPixelShader.hlsl" );
	Gameobject obelisk = {};
	Mesh obMesh;
	modelimporter.init( ( char* )"Obelisk2.obj", obMesh );
	obMesh.init( d3d.getDevice() );
	obelisk.init( d3d.getDevice(), d3d.getDeviceContext(), &camera, &obMat, &obMesh );
	obelisk.SetRotation( XMFLOAT3( 0.0f, 0.0f, 0.0f ) );
	obelisk.SetScale( XMFLOAT3( 0.08f, 0.08f, 0.08f ) );
	obelisk.SetPosition( XMFLOAT3( 0.0f, -1.5f, 2.0f ) );

	// 8. run application

	while (true)
	{
		if (!window.run()) break;

		// 8.1. update objects
		time.update();
	//	mesh.update(time.getDeltaTime());
		plMesh.update(time.getDeltaTime());
		cubeMesh.update( time.getDeltaTime() );
		obelisk.update( time.getDeltaTime() );

		//iMesh.update( time.getDeltaTime() );

		// 8.2. draw objects 
		d3d.beginScene(0.0f, 0.0f, 0.0f);

		//plane.render( d3d.getDeviceContext() );
		model.render( d3d.getDeviceContext(), time.getTime() );
		obelisk.render( d3d.getDeviceContext(), time.getTime() );
		// rendering stuff

		d3d.endScene();
	}

	// 9. tidy up
	material.deInit();
	time.deInit();
	camera.deInit();
	//iMesh.deInit();
	plMesh.deInit();
	obMesh.deInit();
	cubeMesh.deInit();
	d3d.deInit();
	window.deInit();

	return 0;
}