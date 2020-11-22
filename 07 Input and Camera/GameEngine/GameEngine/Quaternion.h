#pragma once

class Vector3;

class Quaternion
{
public:
	Quaternion (float x = 0.0f, float y = 0.0f, float z = 0.0f, float w = 0.0f);
	Quaternion (const Quaternion& other);
	~Quaternion ();

	bool Normalize ();
	bool Inverse ();

	float Magnitude () const;
	float SqrMagnitude () const;
	Quaternion Normalized () const;
	Quaternion Inversed () const;
	Vector3 Euler () const;

	static Quaternion FromEuler (float pitch, float yaw, float roll);
	static Quaternion FromEuler (const Vector3& eulerAngles);

	Quaternion& operator= (const Quaternion& other);
	bool operator== (const Quaternion& other) const;
	bool operator!= (const Quaternion& other) const;
	Quaternion operator+ (const Quaternion& other) const;
	Quaternion operator- (const Quaternion& other) const;
	Quaternion operator* (const Quaternion& other) const;
	Quaternion operator* (const float scale) const;
	Quaternion operator/ (const float scale) const;
	Vector3 operator* (const Vector3& vector) const;
	Quaternion& operator+= (const Quaternion& other);
	Quaternion& operator-= (const Quaternion& other);
	Quaternion& operator*= (const Quaternion& other);
	Quaternion& operator*= (const float scale);
	Quaternion& operator/= (const float scale);

	friend Quaternion operator* (const float scale, const Quaternion& quaternion);

private:
	static float NormalizeAngle (float angle);

public:
	static const Quaternion Identity;

	// Vector (x, y, z) Scalar (w)
	float m_x, m_y, m_z, m_w;
};
