#include "Camera.h"
#include <DirectXMath.h>

using namespace DirectX;

int Camera::init(INT screenWidth, INT screenHeight, XMFLOAT3 camPos)
{
	_cameraForward = { 0.0f, 0.0f, 1.0f, 0.0f };
	_cameraUp = { 0.0f, 1.0f, 0.0f, 0.0f };
	_cameraPos = { camPos.x, camPos.y, camPos.z, 0.0f };
	_camPos = XMLoadFloat3( &camPos );

	// view matrix
	XMMATRIX viewMatrix = XMMatrixLookToLH(
		XMVectorSet( _cameraPos.x, _cameraPos.y, _cameraPos.z, 0 ), // camera position
		XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f), // camera forward direction
		XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f) // camera up direction
	);
	XMStoreFloat4x4(&_viewMatrix, viewMatrix);

	// projection Matrix - perspective
	XMMATRIX projectionMatrix = XMMatrixPerspectiveFovLH(
		XM_PI * 0.3333333f, // field of view in radians
		static_cast<FLOAT>(screenWidth) / static_cast<FLOAT>(screenHeight), // aspect ratio
		0.3f, 1000.0f // near & far clipping
	);
	XMStoreFloat4x4(&_projectionMatrix, projectionMatrix);

	return 0;
}

void Camera::deInit()
{
}

void Camera::RenderReflection( float yPos )
{
	XMFLOAT3 up, position, lookAt;
	float radians;


	// Setup the vector that points upwards.
	up.x = 0.0f;
	up.y = 1.0f;
	up.z = 0.0f;

	XMVECTOR upDir = XMVectorSet( up.x, up.y, up.z, 0 );
	XMVECTOR forwardDir = XMLoadFloat4( &_cameraForward );

	// Setup the position of the camera in the world.
	// For planar reflection invert the Y position of the camera.
	position.x = _cameraPos.x;
	position.y = -_cameraPos.y /*+ ( yPos * 2.0f )*/;
	position.z = _cameraPos.z;

	// Calculate the rotation in radians.
	//radians = m_rotationY * 0.0174532925f;

	// Setup where the camera is looking.
	//lookAt.x = sinf( radians ) + _cameraPos.x;
	//lookAt.y = position.y;
	//lookAt.z = cosf( radians ) + _cameraPos.z;

	FLOAT posY = -_cameraPos.y + ( yPos * 2.0f );

	posY = _cameraPos.y;

		// view matrix
	XMMATRIX viewMatrix = XMMatrixLookToLH(
		XMVectorSet( _cameraPos.x, posY , _cameraPos.z, 0 ), // camera position
		XMVectorSet( 0.0f, 0.0f, 1.0f, 0.0f ), // camera forward direction
		XMVectorSet( 0.0f, 1.0f, 0.0f, 0.0f ) // camera up direction
	);

	// Create the view matrix from the three vectors.
	XMMATRIX refM = XMMatrixLookAtLH( _camPos, forwardDir, upDir );
	
	XMStoreFloat4x4( &_reflectionMatrix, viewMatrix );


	return;
}
