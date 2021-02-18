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
	

	// Plane test
	Vertex planeVertices[] =
	{
		// quad - trianglestrip or trianglelist with index buffer
		// position with normal & uv
		Vertex( -1.0f, -0.2f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f ),
		Vertex( 1.0f, -0.2f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f ),
		Vertex( -1.0f, -0.2f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, -1.0f ),
		Vertex( 1.0f, -0.2f, -1.0f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f )

	};

	WORD planeIndices[] =
	{
		0, 1, 2,
		1, 3, 2

	};

	Vertex cubeVerts[] =
	{
		Vertex ( -.3f, -.3f, -.3f,		0.0f, 0.0f, 1.0f,		0.0f, 0.0f ),
		Vertex ( .3f, -.3f, -.3f,		1.0f, 0.0f, 0.0f,		0.0f, 0.0f ),
		Vertex ( .3f, .3f, -.3f,		1.0f, 0.0f, -1.0f,		0.0f, 0.0f ),
		Vertex ( -.3f, .3f, -.3f,		-1.0f, 0.0f, 0.0f,		0.0f, 0.0f ),
		Vertex ( -.3f, -.3f, .3f,		1.0f, 0.0f, 0.0f,		0.0f, 0.0f ),
		Vertex ( .3f, -.3f, .3f,		1.0f, 1.0f, 0.0f,		0.0f, 0.0f ),
		Vertex ( .3f, .3f, .3f,			1.0f, 0.0f, 0.0f,		0.0f, 0.0f ),
		Vertex ( -.3f, .3f, .3f,		1.0f, -1.0f, 0.0f,		0.0f, 0.0f ),

	};

	WORD cubeIndices[6 * 6] =
	{
		 1, 5, 0, 0, 5, 4,
		 6, 2, 7, 7, 2, 3,
		 3, 0, 7, 7, 0, 4,
		 7, 4, 6, 6, 4, 5,
		 6, 5, 2, 2, 5, 1,
		 2, 1, 3, 3, 1, 0
	};

	//WORD cubeIndices[6 * 6] =
	//{
	//	0, 1, 3, 3, 1, 2,
	//	1, 5, 2, 2, 5, 6,
	//	5, 4, 6, 6, 4, 7,
	//	4, 0, 7, 7, 0, 3,
	//	3, 2, 7, 7, 2, 6,
	//	4, 5, 0, 0, 5, 1
	//};


	Mesh plMesh;
	plMesh.SetVertices( planeVertices, 4 );
	plMesh.SetIndices( planeIndices, 6 );
	plMesh.init( d3d.getDevice() );
	Gameobject plane = {};
	plane.init( d3d.getDevice(), d3d.getDeviceContext(), &camera, &material, &plMesh );
	
	Material cubeMat;
	cubeMat.init( d3d.getDevice(), L"wall.jpg", L"LightVertexShader.hlsl", L"LightPixelShader.hlsl" );
	Mesh cubeMesh;
	cubeMesh.SetVertices( cubeVerts, 8 );
	cubeMesh.SetIndices( cubeIndices, 6 * 6 );
	cubeMesh.init( d3d.getDevice() );

	Gameobject cube = {};
	cube.init( d3d.getDevice(), d3d.getDeviceContext(), &camera, &cubeMat, &cubeMesh );

	//// SKYBOX
	//Skybox skybox;
	// skybox.init();


	//// MODEL IMPORT
	Gameobject model = {};
	Mesh iMesh;
	int vCount;
	Modelmporter modelimporter;
	modelimporter.init( (char*)"basicPlane.obj", vCount, iMesh);
	iMesh.init( d3d.getDevice() );
	model.init( d3d.getDevice(), d3d.getDeviceContext(), &camera, &cubeMat, &iMesh );
	//model.SetPosition( XMFLOAT3( 0.0f, 0.0f, 0.0f ) );
	model.SetScale( XMFLOAT3( 0.05f, 0.05f, 0.05f ) );
	model.SetRotation( XMFLOAT3( 1.0f, 0.0f, 0.0f ) );
	// 8. run application
	while (true)
	{
		if (!window.run()) break;

		// 8.1. update objects
		time.update();
	//	mesh.update(time.getDeltaTime());
		plMesh.update(time.getDeltaTime());
		cubeMesh.update( time.getDeltaTime() );

		iMesh.update( time.getDeltaTime() );

		// 8.2. draw objects 
		d3d.beginScene(0.0f, 0.0f, 0.0f);

		//plane.render( d3d.getDeviceContext() );
		model.render( d3d.getDeviceContext() );
		// rendering stuff

		d3d.endScene();
	}

	// 9. tidy up
	material.deInit();
	time.deInit();
	camera.deInit();
	iMesh.deInit();
	plMesh.deInit();
	cubeMesh.deInit();
	d3d.deInit();
	window.deInit();

	return 0;
}