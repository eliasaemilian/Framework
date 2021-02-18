#include "Material.h"
#include "WICTextureLoader9.h"

using namespace DirectX;

int Material::init(IDirect3DDevice9* pD3DDevice, LPCWSTR textureName)
{
	HRESULT hr = CreateWICTextureFromFile(pD3DDevice, textureName, &_pTexture);
	if (FAILED(hr)) return 40;

	_material.Ambient = { 1.0f, 1.0f, 1.0f, 1.0f };
	_material.Diffuse = { 1.0f, 1.0f, 1.0f, 1.0f };
	_material.Specular = { 0.5f, 0.5f, 0.5f, 1.0f };
	_material.Emissive = { 0.0f, 0.0f, 0.0f, 1.0f };
	_material.Power = 2048.0f;

	return 0;
}

void Material::render(IDirect3DDevice9* pD3DDevice)
{
	pD3DDevice->SetTexture(0, _pTexture);
	pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
	pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSW, D3DTADDRESS_WRAP);
	pD3DDevice->SetSamplerState(0, D3DSAMP_BORDERCOLOR, D3DCOLOR_XRGB(255, 0, 255));
	pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	pD3DDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
	pD3DDevice->SetSamplerState(0, D3DSAMP_MAXANISOTROPY, 16);

	pD3DDevice->SetMaterial(&_material);
}

void Material::deInit()
{
	if (_pTexture != nullptr)
	{
		_pTexture->Release();
		_pTexture = nullptr;
	}
}
