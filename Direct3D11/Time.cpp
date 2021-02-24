#include "Time.h"
#include <Windows.h>

#pragma comment(lib, "winmm.lib")


int Time::init()
{
	_lastTimestamp = timeGetTime() * 0.001f; // convert ms into s

	FLOAT Time = getTime();
	FLOAT SinTime = static_cast< FLOAT > ( sin( Time ) );
	FLOAT CosTime = static_cast< FLOAT > ( cos( Time ) );
	FLOAT DeltaTime = getDeltaTime();

	_shaderParamsTime = XMFLOAT4( Time, SinTime, CosTime, DeltaTime );

	return 0;
}

void Time::update()
{
	float actualTimestamp = timeGetTime() * 0.001f;
	_deltaTime = actualTimestamp - _lastTimestamp;
	_totalTime += _deltaTime;
	_lastTimestamp = actualTimestamp;


	FLOAT Time = getTime();
	FLOAT SinTime = static_cast< FLOAT > ( sin( Time ) );
	FLOAT CosTime = static_cast< FLOAT > ( cos( Time ) );
	FLOAT DeltaTime = getDeltaTime();

	_shaderParamsTime = XMFLOAT4( Time, SinTime, CosTime, DeltaTime );
}

void Time::deInit() {}


