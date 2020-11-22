#pragma once

class Vector3
{
public:
	Vector3 (float x = 0.0f, float y = 0.0f, float z = 0.0f);
	Vector3 (const Vector3& other);
	~Vector3 ();

	bool Normalize ();

	float Magnitude () const;
	float SqrMagnitude () const;
	Vector3 Normalized () const;

	static float Dot (const Vector3& v1, const Vector3& v2);
	static Vector3 Cross (const Vector3& v1, const Vector3& v2);
	static float Distance (const Vector3& v1, const Vector3& v2);

	Vector3& operator= (const Vector3& other);
	bool operator== (const Vector3& other) const;
	bool operator!= (const Vector3& other) const;
	Vector3 operator- () const;
	Vector3 operator+ (const Vector3& other) const;
	Vector3 operator- (const Vector3& other) const;
	Vector3 operator* (const float scale) const;
	Vector3 operator/ (const float scale) const;
	Vector3& operator+= (const Vector3& other);
	Vector3& operator-= (const Vector3& other);
	Vector3& operator*= (const float scale);
	Vector3& operator/= (const float scale);

public:
	static const Vector3 Zero;
	static const Vector3 One;
	static const Vector3 Forward;
	static const Vector3 Back;
	static const Vector3 Right;
	static const Vector3 Left;
	static const Vector3 Up;
	static const Vector3 Down;

	float m_x, m_y, m_z;
};

Vector3 operator* (const float scale, const Vector3& vector);
