#include "Vector4.h"
#include "Math.h"

const Vector4 Vector4::Zero = Vector4 (0.0f, 0.0f, 0.0f, 0.0f);
const Vector4 Vector4::One = Vector4 (1.0f, 1.0f, 1.0f, 1.0f);

Vector4::Vector4 (float x, float y, float z, float w) :
	m_x (x), m_y (y), m_z (z), m_w (w)
{
}

Vector4::Vector4 (const Vector4& other) :
	m_x (other.m_x), m_y (other.m_y), m_z (other.m_z), m_w (other.m_w)
{
}

Vector4::~Vector4 ()
{
}

bool Vector4::Normalize ()
{
	const float squaredMagnitude = SqrMagnitude ();

	if (squaredMagnitude > Math::SmallValue)
	{
		const float scale = 1.0f / Math::Sqrt (squaredMagnitude);

		m_x *= scale;
		m_y *= scale;
		m_z *= scale;
		m_w *= scale;

		return true;
	}

	return false;
}

float Vector4::Magnitude () const
{
	return Math::Sqrt (m_x * m_x + m_y * m_y + m_z * m_z + m_w * m_w);
}

float Vector4::SqrMagnitude () const
{
	return m_x * m_x + m_y * m_y + m_z * m_z + m_w * m_w;
}

Vector4 Vector4::Normalized () const
{
	const float squaredMagnitude = SqrMagnitude ();

	if (squaredMagnitude > Math::SmallValue)
	{
		const float scale = 1.0f / Math::Sqrt (squaredMagnitude);

		return Vector4 (m_x * scale, m_y * scale, m_z * scale, m_w * scale);
	}

	return Zero;
}

float Vector4::Dot (const Vector4& v1, const Vector4& v2)
{
	return (v1.m_x * v2.m_x) + (v1.m_y * v2.m_y) + (v1.m_z * v2.m_z) + (v1.m_w * v2.m_w);
}

float Vector4::Distance (const Vector4& v1, const Vector4& v2)
{
	return (v1 - v2).Magnitude ();
}

Vector4& Vector4::operator= (const Vector4& other)
{
	m_x = other.m_x;
	m_y = other.m_y;
	m_z = other.m_z;
	m_w = other.m_w;

	return *this;
}

bool Vector4::operator== (const Vector4& other) const
{
	return (m_x == other.m_x) && (m_y == other.m_y) && (m_z == other.m_z) && (m_w == other.m_w);
}

bool Vector4::operator!= (const Vector4& other) const
{
	return (m_x != other.m_x) || (m_y != other.m_y) || (m_z != other.m_z) || (m_w != other.m_w);
}

Vector4 Vector4::operator- () const
{
	return -1.0f * (*this);
}

Vector4 Vector4::operator+ (const Vector4& other) const
{
	return Vector4 (m_x + other.m_x, m_y + other.m_y, m_z + other.m_z, m_w + other.m_w);
}

Vector4 Vector4::operator- (const Vector4& other) const
{
	return Vector4 (m_x - other.m_x, m_y - other.m_y, m_z - other.m_z, m_w - other.m_w);
}

Vector4 Vector4::operator* (const float scale) const
{
	return Vector4 (m_x * scale, m_y * scale, m_z * scale, m_w * scale);
}

Vector4 Vector4::operator/ (const float scale) const
{
	return Vector4 (m_x / scale, m_y / scale, m_z / scale, m_w / scale);
}

Vector4& Vector4::operator+= (const Vector4& other)
{
	m_x += other.m_x;
	m_y += other.m_y;
	m_z += other.m_z;
	m_w += other.m_w;

	return *this;
}

Vector4& Vector4::operator-= (const Vector4& other)
{
	m_x -= other.m_x;
	m_y -= other.m_y;
	m_z -= other.m_z;
	m_w -= other.m_w;

	return *this;
}

Vector4& Vector4::operator*= (const float scale)
{
	m_x *= scale;
	m_y *= scale;
	m_z *= scale;
	m_w *= scale;

	return *this;
}

Vector4& Vector4::operator/= (const float scale)
{
	m_x /= scale;
	m_y /= scale;
	m_z /= scale;
	m_w /= scale;

	return *this;
}

Vector4 operator* (const float scale, const Vector4& vector)
{
	return Vector4 (scale * vector.m_x, scale * vector.m_y, scale * vector.m_z, scale * vector.m_w);
}