#pragma once

class Math
{
public:
	static float  Sqrt (float value);
	static double Sqrt (double value);

	static float  Sin (float radians);
	static double Sin (double radians);

	static float  Cos (float radians);
	static double Cos (double radians);

	static float  ArcSin (float value);
	static double ArcSin (double value);

	static float  ArcCos (float value);
	static double ArcCos (double value);

	static float  ArcTan2 (float y, float x);
	static double ArcTan2 (double y, double x);

	static float  Remainder (float value, float denominator);
	static double Remainder (double value, double denominator);

public:
	static const float SmallValue;
	static const float Pi;
	static const float RcpPi;
	static const float Deg2Rad;
	static const float Rad2Deg;
};

