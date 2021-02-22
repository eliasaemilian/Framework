#pragma once
#include <d3d11.h>
#include <DirectXMath.h>

using namespace DirectX;

class Camera
{
public:
	int init( INT screenWidth, INT screenHeight, XMFLOAT3 pos );
	void deInit();

	XMFLOAT4X4* getViewMatrix() { return &_viewMatrix; }
	XMFLOAT4X4* getProjectionMatrix() { return &_projectionMatrix; }

	void RenderReflection( float yPos );
	XMFLOAT4X4* getReflectionMatrix() { return &_reflectionMatrix; }
	XMFLOAT4* getCameraPos() { return &_cameraPos; }

private:
	XMFLOAT4X4 _viewMatrix = {};
	XMFLOAT4X4 _projectionMatrix = {};
	XMFLOAT4X4 _reflectionMatrix = {};

	XMFLOAT4 _cameraPos;
	XMFLOAT4 _cameraForward;
	XMFLOAT4 _cameraUp;
};

