#include "Camera.h"
#include <DirectXMath.h>

using namespace DirectX;

#define NEAR_CLIP_PLANE 0.3
#define FAR_CLIP_PLANE 1000

int Camera::init( INT screenWidth, INT screenHeight, XMFLOAT3 camPos )
{
	_cameraForward = { 0.0f, 0.0f, 1.0f, 0.0f };
	_cameraUp = { 0.0f, 1.0f, 0.0f, 0.0f };
	_cameraPos = { camPos.x, camPos.y, camPos.z, 0.0f };

	// view matrix
	XMMATRIX viewMatrix = XMMatrixLookToLH(
		XMVectorSet( _cameraPos.x, _cameraPos.y, _cameraPos.z, _cameraPos.w ), // camera position
		XMVectorSet( _cameraForward.x, _cameraForward.y, _cameraForward.z, _cameraForward.w ), // camera forward direction
		XMVectorSet( _cameraUp.x, _cameraUp.y, _cameraUp.z, _cameraUp.w ) // camera up direction
	);
	XMStoreFloat4x4( &_viewMatrix, viewMatrix );

	// projection Matrix - perspective
	XMMATRIX projectionMatrix = XMMatrixPerspectiveFovLH(
		XM_PI * 0.3333333f, // field of view in radians
		static_cast< FLOAT >( screenWidth ) / static_cast< FLOAT >( screenHeight ), // aspect ratio
		static_cast< FLOAT > ( NEAR_CLIP_PLANE ), static_cast< FLOAT > ( FAR_CLIP_PLANE ) // near & far clipping
	);
	XMStoreFloat4x4( &_projectionMatrix, projectionMatrix );

	return 0;
}

void Camera::deInit()
{
}

void Camera::RenderReflection( float yPos )
{
	FLOAT posY = -_cameraPos.y + ( yPos * 2.0f );

	// view matrix
	XMMATRIX viewMatrix = XMMatrixLookToLH(
		XMVectorSet( _cameraPos.x, posY, _cameraPos.z, 0 ), // camera position
		XMVectorSet( _cameraForward.x, _cameraForward.y, _cameraForward.z, 0.0f ), // camera forward direction
		XMVectorSet( 0.0f, 1.0f, 0.0f, 0.0f ) // camera up direction
	);

	// generate reflection matrix
	XMMATRIX refM = XMMatrixLookAtLH( XMLoadFloat4( &_cameraPos ), XMLoadFloat4( &_cameraForward ), XMLoadFloat4( &_cameraUp ) );

	XMStoreFloat4x4( &_reflectionMatrix, viewMatrix );

	return;
}
