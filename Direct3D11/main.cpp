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

#define SCREEN_WIDTH  1280
#define SCREEN_HEIGHT 1024


int CALLBACK WinMain(
	_In_	 HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_	 LPSTR     lpCmdLine,
	_In_	 int       nCmdShow )
{
	INT width = SCREEN_WIDTH;
	INT height = SCREEN_HEIGHT;
	bool isWindowed = true;

	// INIT WINDOW
	Window window;
	int error = window.init( hInstance, width, height, nCmdShow );
	if (error != 0) return error;

	// INIT DIRECT X
	D3D d3d;
	error = d3d.init( window.getWindowHandle(), width, height, isWindowed );
	if (error != 0) return error;

	// INIT SCENE
	Scene scene = {};
	error = scene.init( d3d.getDevice(), d3d.getDeviceContext(), d3d.getDepthStencilView(), width, height );
	if (error != 0) return error;


	// MESSAGE LOOP
	while (true)
	{
		if (!window.run()) break;

		// UPDATE
		scene.update();

		// CLEAR SCENE 
		d3d.clearRenderTarget( 0.0f, 0.0f, 0.0f );

		// RENDER CALL
		scene.ReflectionsRenderpass();

		// set render target to backbuffer
		d3d.setBackBufferRenderTarget();
		d3d.clearRenderTarget( 0.0f, 0.0f, 0.0f );

		// render geometry
		scene.GeometryRenderpass();

		// render z write off
		d3d.disableDrawToDepthStencil();
		scene.ZWriteOffRenderpass();

		// SWITCH BUFFERS
		d3d.endScene();
	}

	// CLEAR MEMORY
	scene.deInit();
	d3d.deInit();
	window.deInit();

	return 0;
}