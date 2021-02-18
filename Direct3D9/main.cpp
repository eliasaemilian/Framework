#include <Windows.h>
#include "Window.h"
#include "D3D.h"
#include "Mesh.h"
#include "Camera.h"
#include "Time.h"
#include "Material.h"
#include "Light.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR szCmdLine, int nCmdShow)
{
	INT width = 800;
	INT height = 600;
	bool isWindowed = true;

	// 1. create window
	Window window;
	int error = window.init(hInstance, width, height, nCmdShow);
	if (error != 0) return error;

	// 2. create Direct3D 9 interface
	D3D d3d;
	error = d3d.init(window.getWindowHandle(), width, height, isWindowed);
	if (error != 0) return error;

	d3d.getDevice()->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	d3d.getDevice()->SetRenderState(D3DRS_LIGHTING, true);
	d3d.getDevice()->SetRenderState(D3DRS_SPECULARENABLE, true);
	d3d.getDevice()->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	// 3. create mesh
	Mesh mesh;
	error = mesh.init(d3d.getDevice());
	if (error != 0) return error;

	// 4. create camera
	Camera camera;
	error = camera.init(width, height);
	if (error != 0) return error;

	// 5. create time
	Time time;
	error = time.init();
	if (error != 0) return error;

	// 6. create material
	Material material;
	error = material.init(d3d.getDevice(), L"wall.jpg");
	if (error != 0) return error;

	// 7. create light
	Light light;
	error = light.init();
	if (error != 0) return error;

	// 8. run application
	while (true)
	{
		if (!window.run()) break;

		// 8.1. update objects
		time.update();
		mesh.update(time.getDeltaTime());

		// 8.2. draw objects 
		d3d.beginScene(D3DCOLOR_XRGB(255, 255, 255));

		// rendering stuff
		camera.render(d3d.getDevice());
		material.render(d3d.getDevice());
		light.render(d3d.getDevice());
		mesh.render(d3d.getDevice());

		d3d.endScene();
	}

	// 9. tidy up
	light.deInit();
	material.deInit();
	time.deInit();
	camera.deInit();
	mesh.deInit();
	d3d.deInit();
	window.deInit();

	return 0;
}