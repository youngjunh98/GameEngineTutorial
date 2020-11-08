#include "Timer.h"

Timer::Timer () :
	m_time (0.0f), m_deltaTime (0.0f),
	m_tickInterval (0.0f), m_resetTick (), m_previousTick ()
{
	// 1초에 틱이 몇번인지 가져온다.
	LARGE_INTEGER ticksPerSecond;
	QueryPerformanceFrequency (&ticksPerSecond);

	// 틱 1번에 몇초인지 계산한다.
	m_tickInterval = 1.0f / static_cast<float> (ticksPerSecond.QuadPart);
}

Timer::~Timer ()
{
}

void Timer::Reset ()
{
	m_time = 0.0f;
	m_deltaTime = 0.0f;

	// 타이머의 기준을 현재로 설정한다.
	m_resetTick = GetTick ();
	m_previousTick = m_resetTick;
}

void Timer::Tick ()
{
	// 현재 틱을 가져온다.
	LARGE_INTEGER tick = GetTick ();

	// 현재 시간과 Delta time을 계산한다.
	m_time = static_cast<float> (tick.QuadPart - m_resetTick.QuadPart) * m_tickInterval;
	m_deltaTime = static_cast<float> (tick.QuadPart - m_previousTick.QuadPart) * m_tickInterval;

	if (m_deltaTime < 0.0f)
	{
		m_deltaTime = 0.0f;
	}

	// 이전 틱에 현재 틱을 저장한다.
	m_previousTick = tick;
}

float Timer::GetTime () const
{
	return m_time;
}

float Timer::GetDeltaTime () const
{
	return m_deltaTime;
}

LARGE_INTEGER Timer::GetTick () const
{
	LARGE_INTEGER tick;
	QueryPerformanceCounter (&tick);

	return tick;
}
