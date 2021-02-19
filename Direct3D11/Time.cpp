#include "Time.h"
#include <Windows.h>
#include <string>

#pragma comment(lib, "Winmm.lib")

using namespace std;

int Time::init()
{
	_lastTimestamp = timeGetTime() * 0.001f; // convert ms into s

	return 0;
}

void Time::update()
{
	float actualTimestamp = timeGetTime() * 0.001f;
	_deltaTime = actualTimestamp - _lastTimestamp;
	_totalTime += _deltaTime;
	_lastTimestamp = actualTimestamp;


}

void Time::deInit()
{
}

float Time::getTime()
{
	return _totalTime;
}
