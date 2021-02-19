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
#include "SkyboxMaterial.h"

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 1024

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR szCmdLine, int nCmdShow)
{
	INT width = SCREEN_WIDTH;
	INT height = SCREEN_HEIGHT;
	bool isWindowed = true;

	// 1. create window
	Window window;
	int error = window.init(hInstance, width, height, nCmdShow);
	if (error != 0) return error;

	// 2. create Direct3D 11 interface
	D3D d3d;
	error = d3d.init(window.getWindowHandle(), width, height, isWindowed);
	if (error != 0) return error;


	// 4. create camera
	Camera camera;
	error = camera.init(width, height);
	if (error != 0) return error;

	// 5. create time
	Time time;
	error = time.init();
	if (error != 0) return error;



	// SCENE
	Scene scene = {};
	error = scene.init( d3d.getDevice(), d3d.getDeviceContext(), &camera );
	if (error != 0) return error;




	// 8. run application

	while (true)
	{
		if (!window.run()) break;

		// 8.1. update objects
		time.update();
		scene.update( time.getDeltaTime() );

		// 8.2. draw objects 
		d3d.beginScene(0.0f, 0.0f, 0.0f);

		scene.render( time.getTime() );
		// rendering stuff

		d3d.endScene();
	}

	// 9. tidy up
	scene.deInit();
	time.deInit();
	camera.deInit();
	//obMesh.deInit();
	
	//  TODO: DEINIT ALL MATS, MESHES //

	//mesh->deInit();
	//material->deInit();


	d3d.deInit();
	window.deInit();

	return 0;
}