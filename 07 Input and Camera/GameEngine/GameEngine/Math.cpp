#include <cmath>

#include "Math.h"

const float Math::SmallValue = 0.0000000001f;
const float Math::Pi = 3.141592653f;
const float Math::RcpPi = 0.318309886f;
const float Math::Deg2Rad = Math::Pi / 180.0f;
const float Math::Rad2Deg = 180.0f / Math::Pi;

float Math::Sqrt (float value)
{
	return sqrt (value);
}

double Math::Sqrt (double value)
{
	return sqrt (value);
}

float Math::Sin (float radians)
{
	return sin (radians);
}

double Math::Sin (double radians)
{
	return sin (radians);
}

float Math::Cos (float radians)
{
	return cos (radians);
}

double Math::Cos (double radians)
{
	return cos (radians);
}

float Math::ArcSin (float value)
{
	return asin (value);
}

double Math::ArcSin (double value)
{
	return asin (value);
}

float Math::ArcCos (float value)
{
	return acos (value);
}

double Math::ArcCos (double value)
{
	return acos (value);
}

float Math::ArcTan2 (float y, float x)
{
	return atan2 (y, x);
}

double Math::ArcTan2 (double y, double x)
{
	return atan2 (y, x);
}

float Math::Remainder (float value, float denominator)
{
	return fmod (value, denominator);
}

double Math::Remainder (double value, double denominator)
{
	return fmod (value, denominator);
}
