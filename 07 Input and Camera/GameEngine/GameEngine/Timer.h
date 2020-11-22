#pragma once

#include <Windows.h>

class Timer
{
public:
	Timer ();
	virtual ~Timer ();

	void Reset ();
	void Tick ();

	float GetTime () const;
	float GetDeltaTime () const;

protected:
	LARGE_INTEGER GetTick () const;

private:
	float m_time;
	float m_deltaTime;

	float m_tickInterval;
	LARGE_INTEGER m_resetTick;
	LARGE_INTEGER m_previousTick;
};
