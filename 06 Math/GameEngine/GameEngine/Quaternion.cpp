#include "Quaternion.h"
#include "Math.h"
#include "Vector3.h"

const Quaternion Quaternion::Identity = Quaternion (0.0f, 0.0f, 0.0f, 1.0f);

Quaternion::Quaternion (float x, float y, float z, float w) :
	m_x (x), m_y (y), m_z (z), m_w (w)
{
}

Quaternion::Quaternion (const Quaternion& other) :
	m_x (other.m_x), m_y (other.m_y), m_z (other.m_z), m_w (other.m_w)
{
}

Quaternion::~Quaternion ()
{
}

bool Quaternion::Normalize ()
{
	const float squaredMagnitude = SqrMagnitude ();

	if (squaredMagnitude > Math::SmallValue)
	{
		float scale = 1.0f / Math::Sqrt (squaredMagnitude);

		m_x *= scale;
		m_y *= scale;
		m_z *= scale;
		m_w *= scale;

		return true;
	}

	return false;
}

bool Quaternion::Inverse ()
{
	const float squaredMagnitude = SqrMagnitude ();

	if (squaredMagnitude > Math::SmallValue)
	{
		float scale = 1.0f / squaredMagnitude;

		m_x *= -scale;
		m_y *= -scale;
		m_z *= -scale;
		m_w *= scale;

		return true;
	}

	return false;
}

float Quaternion::Magnitude () const
{
	return Math::Sqrt (m_x * m_x + m_y * m_y + m_z * m_z + m_w * m_w);
}

float Quaternion::SqrMagnitude () const
{
	return m_x * m_x + m_y * m_y + m_z * m_z + m_w * m_w;
}

Quaternion Quaternion::Normalized () const
{
	const float squaredMagnitude = SqrMagnitude ();

	if (squaredMagnitude > Math::SmallValue)
	{
		float scale = 1.0f / Math::Sqrt (squaredMagnitude);

		return Quaternion (m_x * scale, m_y * scale, m_z * scale, m_w * scale);
	}

	return Identity;
}

Quaternion Quaternion::Inversed () const
{
	const float squaredMagnitude = SqrMagnitude ();

	if (squaredMagnitude > Math::SmallValue)
	{
		float scale = 1.0f / squaredMagnitude;

		return Quaternion (-m_x * scale, -m_y * scale, -m_z * scale, m_w * scale);
	}

	return Identity;
}

Vector3 Quaternion::Euler () const
{
	const float singularity = 0.4999995f;

	const float x2 = m_x * m_x;
	const float y2 = m_y * m_y;
	const float z2 = m_z * m_z;
	const float w2 = m_w * m_w;

	const float test = m_x * m_w - m_y * m_z;
	const float unit = x2 + y2 + z2 + w2;

	float pitch = 0.0f;
	float yaw = 0.0f;
	float roll = 0.0f;

	if (test > singularity * unit)
	{
		pitch = 90.0f;
		yaw = 2.0f * Math::ArcTan2 (m_y, m_x);
		roll = 0.0f;
	}
	else if (test < -singularity * unit)
	{
		pitch = -90.0f;
		yaw = -2.0f * Math::ArcTan2 (m_y, m_x);
		roll = 0.0f;
	}
	else
	{
		pitch = Math::ArcSin (2.0f * test);
		yaw = Math::ArcTan2 (2.0f * m_y * m_w + 2.0f * m_x * m_z, 1.0f - 2.0f * (x2 + y2));
		roll = Math::ArcTan2 (2.0f * m_z * m_w + 2.0f * m_x * m_y, 1.0f - 2.0f * (x2 + z2));
	}

	pitch = NormalizeAngle (pitch * Math::Rad2Deg);
	yaw = NormalizeAngle (yaw * Math::Rad2Deg);
	roll = NormalizeAngle (roll * Math::Rad2Deg);

	return Vector3 (pitch, yaw, roll);
}

Quaternion Quaternion::FromEuler (float pitch, float yaw, float roll)
{
	const float pitchHalfRadians = NormalizeAngle (pitch * 0.5f) * Math::Deg2Rad;
	const float yawHalfRadians = NormalizeAngle (yaw * 0.5f) * Math::Deg2Rad;
	const float rollHalfRadians = NormalizeAngle (roll * 0.5f) * Math::Deg2Rad;

	const float sinPitch = Math::Sin (pitchHalfRadians);
	const float sinYaw = Math::Sin (yawHalfRadians);
	const float sinRoll = Math::Sin (rollHalfRadians);

	const float cosPitch = Math::Cos (pitchHalfRadians);
	const float cosYaw = Math::Cos (yawHalfRadians);
	const float cosRoll = Math::Cos (rollHalfRadians);

	// Multiplied in yxz order
	return Quaternion (
		sinPitch * cosYaw * cosRoll + cosPitch * sinYaw * sinRoll,
		cosPitch * sinYaw * cosRoll - sinPitch * cosYaw * sinRoll,
		cosPitch * cosYaw * sinRoll - sinPitch * sinYaw * cosRoll,
		cosPitch * cosYaw * cosRoll + sinPitch * sinYaw * sinRoll
	);
}

Quaternion Quaternion::FromEuler (const Vector3& eulerAngles)
{
	return FromEuler (eulerAngles.m_x, eulerAngles.m_y, eulerAngles.m_z);
}

Quaternion& Quaternion::operator= (const Quaternion& other)
{
	m_x = other.m_x;
	m_y = other.m_y;
	m_z = other.m_z;
	m_w = other.m_w;

	return *this;
}

bool Quaternion::operator== (const Quaternion& other) const
{
	return (m_x == other.m_x) && (m_y == other.m_y) && (m_z == other.m_z) && (m_w == other.m_w);
}

bool Quaternion::operator!= (const Quaternion& other) const
{
	return (m_x != other.m_x) || (m_y != other.m_y) || (m_z != other.m_z) || (m_w != other.m_w);
}

Quaternion Quaternion::operator+ (const Quaternion& other) const
{
	return Quaternion (m_x + other.m_x, m_y + other.m_y, m_z + other.m_z, m_w + other.m_w);
}

Quaternion Quaternion::operator- (const Quaternion& other) const
{
	return Quaternion (m_x - other.m_x, m_y - other.m_y, m_z - other.m_z, m_w - other.m_w);
}

Quaternion Quaternion::operator* (const Quaternion& other) const
{
	return Quaternion (
		m_w * other.m_x + other.m_w * m_x + m_y * other.m_z - m_z * other.m_y,
		m_w * other.m_y + other.m_w * m_y + m_z * other.m_x - m_x * other.m_z,
		m_w * other.m_z + other.m_w * m_z + m_x * other.m_y - m_y * other.m_x,
		m_w * other.m_w - m_x * other.m_x - m_y * other.m_y - m_z * other.m_z
	);
}

Quaternion Quaternion::operator* (const float scale) const
{
	return Quaternion (m_x * scale, m_y * scale, m_z * scale, m_w * scale);
}

Quaternion Quaternion::operator/ (const float scale) const
{
	return Quaternion (m_x / scale, m_y / scale, m_z / scale, m_w / scale);
}

Vector3 Quaternion::operator* (const Vector3& vector) const
{
	const Vector3 u (m_x, m_y, m_z);

	const Vector3 v1 = (2.0f * m_w * m_w - 1.0f) * vector;
	const Vector3 v2 = 2.0f * Vector3::Dot (u, vector) * u;
	const Vector3 v3 = 2.0f * m_w * Vector3::Cross (u, vector);

	return v1 + v2 + v3;
}

Quaternion& Quaternion::operator+= (const Quaternion& other)
{
	m_x += other.m_x;
	m_y += other.m_y;
	m_z += other.m_z;
	m_w += other.m_w;

	return *this;
}

Quaternion& Quaternion::operator-= (const Quaternion& other)
{
	m_x -= other.m_x;
	m_y -= other.m_y;
	m_z -= other.m_z;
	m_w -= other.m_w;

	return *this;
}

Quaternion& Quaternion::operator*= (const Quaternion& other)
{
	const float x = m_w * other.m_x + other.m_w * m_x + m_y * other.m_z - m_z * other.m_y;
	const float y = m_w * other.m_y + other.m_w * m_y + m_z * other.m_x - m_x * other.m_z;
	const float z = m_w * other.m_z + other.m_w * m_z + m_x * other.m_y - m_y * other.m_x;
	const float w = m_w * other.m_w - m_x * other.m_x - m_y * other.m_y - m_z * other.m_z;

	m_x = x;
	m_y = y;
	m_z = z;
	m_w = w;

	return *this;
}

Quaternion& Quaternion::operator*= (const float scale)
{
	m_x *= scale;
	m_y *= scale;
	m_z *= scale;
	m_w *= scale;

	return *this;
}

Quaternion& Quaternion::operator/= (const float scale)
{
	m_x /= scale;
	m_y /= scale;
	m_z /= scale;
	m_w /= scale;

	return *this;
}

float Quaternion::NormalizeAngle (float angle)
{
	float normalized = Math::Remainder (angle, 360.0f);

	if (normalized < 0.0f)
	{
		normalized += 360.0f;
	}

	return normalized;
}

Quaternion operator*(const float scale, const Quaternion& quaternion)
{
	return Quaternion (quaternion.m_x * scale, quaternion.m_y * scale, quaternion.m_z * scale, quaternion.m_w * scale);
}
