#include "Vector3.h"
#include "Math.h"

const Vector3 Vector3::Zero = Vector3 (0.0f, 0.0f, 0.0f);
const Vector3 Vector3::One = Vector3 (1.0f, 1.0f, 1.0f);
const Vector3 Vector3::Forward = Vector3 (0.0f, 0.0f, 1.0f);
const Vector3 Vector3::Back = Vector3 (0.0f, 0.0f, -1.0f);
const Vector3 Vector3::Right = Vector3 (1.0f, 0.0f, 0.0f);
const Vector3 Vector3::Left = Vector3 (-1.0f, 0.0f, 0.0f);
const Vector3 Vector3::Up = Vector3 (0.0f, 1.0f, 0.0f);
const Vector3 Vector3::Down = Vector3 (0.0f, -1.0f, 0.0f);

Vector3::Vector3 (float x, float y, float z) :
	m_x (x), m_y (y), m_z (z)
{
}

Vector3::Vector3 (const Vector3& other) :
	m_x (other.m_x), m_y (other.m_y), m_z (other.m_z)
{
}

Vector3::~Vector3 ()
{
}

bool Vector3::Normalize ()
{
	const float squaredMagnitude = SqrMagnitude ();

	if (squaredMagnitude > Math::SmallValue)
	{
		const float scale = 1.0f / Math::Sqrt (squaredMagnitude);

		m_x *= scale;
		m_y *= scale;
		m_z *= scale;

		return true;
	}

	return false;
}

float Vector3::Magnitude () const
{
	return Math::Sqrt (m_x * m_x + m_y * m_y + m_z * m_z);
}

float Vector3::SqrMagnitude () const
{
	return m_x * m_x + m_y * m_y + m_z * m_z;
}

Vector3 Vector3::Normalized () const
{
	const float squaredMagnitude = SqrMagnitude ();

	if (squaredMagnitude > Math::SmallValue)
	{
		const float scale = 1.0f / Math::Sqrt (squaredMagnitude);

		return Vector3 (m_x * scale, m_y * scale, m_z * scale);
	}

	return Zero;
}

float Vector3::Dot (const Vector3& v1, const Vector3& v2)
{
	return (v1.m_x * v2.m_x) + (v1.m_y * v2.m_y) + (v1.m_z * v2.m_z);
}

Vector3 Vector3::Cross (const Vector3& v1, const Vector3& v2)
{
	const float x = v1.m_y * v2.m_z - v1.m_z * v2.m_y;
	const float y = v1.m_z * v2.m_x - v1.m_x * v2.m_z;
	const float z = v1.m_x * v2.m_y - v1.m_y * v2.m_x;

	return Vector3 (x, y, z);
}

float Vector3::Distance (const Vector3& v1, const Vector3& v2)
{
	return (v1 - v2).Magnitude ();
}

Vector3& Vector3::operator= (const Vector3& other)
{
	m_x = other.m_x;
	m_y = other.m_y;
	m_z = other.m_z;

	return *this;
}

bool Vector3::operator== (const Vector3& other) const
{
	return (m_x == other.m_x) && (m_y == other.m_y) && (m_z == other.m_z);
}

bool Vector3::operator!= (const Vector3& other) const
{
	return (m_x != other.m_x) || (m_y != other.m_y) || (m_z != other.m_z);
}

Vector3 Vector3::operator- () const
{
	return -1.0f * (*this);
}

Vector3 Vector3::operator+ (const Vector3& other) const
{
	return Vector3 (m_x + other.m_x, m_y + other.m_y, m_z + other.m_z);
}

Vector3 Vector3::operator- (const Vector3& other) const
{
	return Vector3 (m_x - other.m_x, m_y - other.m_y, m_z - other.m_z);
}

Vector3 Vector3::operator* (const float scale) const
{
	return Vector3 (m_x * scale, m_y * scale, m_z * scale);
}

Vector3 Vector3::operator/ (const float scale) const
{
	return Vector3 (m_x / scale, m_y / scale, m_z / scale);
}

Vector3& Vector3::operator+= (const Vector3& other)
{
	m_x += other.m_x;
	m_y += other.m_y;
	m_z += other.m_z;

	return *this;
}

Vector3& Vector3::operator-= (const Vector3& other)
{
	m_x -= other.m_x;
	m_y -= other.m_y;
	m_z -= other.m_z;

	return *this;
}

Vector3& Vector3::operator*= (const float scale)
{
	m_x *= scale;
	m_y *= scale;
	m_z *= scale;

	return *this;
}

Vector3& Vector3::operator/= (const float scale)
{
	m_x /= scale;
	m_y /= scale;
	m_z /= scale;

	return *this;
}

Vector3 operator* (const float scale, const Vector3& vector)
{
	return Vector3 (scale * vector.m_x, scale * vector.m_y, scale * vector.m_z);
}