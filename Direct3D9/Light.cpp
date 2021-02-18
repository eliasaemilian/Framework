#include "Light.h"

int Light::init()
{
	_light.Type = D3DLIGHT_DIRECTIONAL;
	_light.Direction = { 1.0f, -1.0f, 1.0f };
	_light.Ambient = { 0.2f, 0.2f, 0.2f, 1.0f };
	_light.Diffuse = { 0.6f, 0.3f, 0.0f, 1.0f };
	_light.Specular = { 1.0f, 1.0f, 1.0f, 1.0f };

	return 0;
}

void Light::render(IDirect3DDevice9* pD3DDevice)
{
	pD3DDevice->SetLight(0, &_light);
	pD3DDevice->LightEnable(0, true);
}

void Light::deInit()
{
}
