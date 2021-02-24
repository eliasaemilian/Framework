#pragma once
#include <DirectXMath.h>


using namespace DirectX;

class Time
{
public:
	int init();
	void update();
	void deInit();

	float getTime() { return _totalTime; };
	float getDeltaTime() { return _deltaTime; }
	XMFLOAT4* getShaderTimeParam() { return &_shaderParamsTime; }

private:
	float _deltaTime = 0.0f;
	float _lastTimestamp = 0.0f;
	float _totalTime = 0.0f;

	XMFLOAT4 _shaderParamsTime = XMFLOAT4( 0,0,0,0 );
};

