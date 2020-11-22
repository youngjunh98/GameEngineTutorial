#pragma once

class Vector4
{
public:
	Vector4 (float x = 0.0f, float y = 0.0f, float z = 0.0f, float w = 0.0f);
	Vector4 (const Vector4& other);
	~Vector4 ();

	bool Normalize ();

	float Magnitude () const;
	float SqrMagnitude () const;
	Vector4 Normalized () const;

	static float Dot (const Vector4& v1, const Vector4& v2);
	static float Distance (const Vector4& v1, const Vector4& v2);

	Vector4& operator= (const Vector4& other);
	bool operator== (const Vector4& other) const;
	bool operator!= (const Vector4& other) const;
	Vector4 operator- () const;
	Vector4 operator+ (const Vector4& other) const;
	Vector4 operator- (const Vector4& other) const;
	Vector4 operator* (const float scale) const;
	Vector4 operator/ (const float scale) const;
	Vector4& operator+= (const Vector4& other);
	Vector4& operator-= (const Vector4& other);
	Vector4& operator*= (const float scale);
	Vector4& operator/= (const float scale);

public:
	static const Vector4 Zero;
	static const Vector4 One;

	float m_x, m_y, m_z, m_w;
};

Vector4 operator* (const float scale, const Vector4& vector);
