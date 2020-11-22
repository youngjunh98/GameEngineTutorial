#include "Matrix4x4.h"
#include "Math.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Quaternion.h"

const Matrix4x4 Matrix4x4::Zero = Matrix4x4 (
	0.0f, 0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f, 0.0f
);

const Matrix4x4 Matrix4x4::Identity = Matrix4x4 (
	1.0f, 0.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 0.0f, 1.0f
);

Matrix4x4::Matrix4x4 (
	float m00, float m01, float m02, float m03,
	float m10, float m11, float m12, float m13,
	float m20, float m21, float m22, float m23,
	float m30, float m31, float m32, float m33) :
	m_values { m00, m01, m02, m03, m10, m11, m12, m13, m20, m21, m22, m23, m30, m31, m32, m33 }
{
}

Matrix4x4::Matrix4x4 (const Matrix4x4& other) : m_values {
	other.m_values[0][0], other.m_values[0][1], other.m_values[0][2], other.m_values[0][3],
	other.m_values[1][0], other.m_values[1][1], other.m_values[1][2], other.m_values[1][3],
	other.m_values[2][0], other.m_values[2][1], other.m_values[2][2], other.m_values[2][3],
	other.m_values[3][0], other.m_values[3][1], other.m_values[3][2], other.m_values[3][3] }
{
}

Matrix4x4::~Matrix4x4 ()
{
}

float Matrix4x4::Determinant () const
{
	return m_values[0][0] * (
		m_values[1][1] * (m_values[2][2] * m_values[3][3] - m_values[2][3] * m_values[3][2]) -
		m_values[1][2] * (m_values[2][1] * m_values[3][3] - m_values[2][3] * m_values[3][1]) +
		m_values[1][3] * (m_values[2][1] * m_values[3][2] - m_values[2][2] * m_values[3][1])
	) - m_values[0][1] * (
		m_values[1][0] * (m_values[2][2] * m_values[3][3] - m_values[2][3] * m_values[3][2]) -
		m_values[1][2] * (m_values[2][0] * m_values[3][3] - m_values[2][3] * m_values[3][0]) +
		m_values[1][3] * (m_values[2][0] * m_values[3][2] - m_values[2][2] * m_values[3][0])
	) + m_values[0][2] * (
		m_values[1][0] * (m_values[2][1] * m_values[3][3] - m_values[2][3] * m_values[3][1]) -
		m_values[1][1] * (m_values[2][0] * m_values[3][3] - m_values[2][3] * m_values[3][0]) +
		m_values[1][3] * (m_values[2][0] * m_values[3][1] - m_values[2][1] * m_values[3][0])
	) - m_values[0][3] * (
		m_values[1][0] * (m_values[2][1] * m_values[3][2] - m_values[2][2] * m_values[3][1]) -
		m_values[1][1] * (m_values[2][0] * m_values[3][2] - m_values[2][2] * m_values[3][0]) +
		m_values[1][2] * (m_values[2][0] * m_values[3][1] - m_values[2][1] * m_values[3][0])
	);
}

Matrix4x4 Matrix4x4::Inversed () const
{
	const float determinant = Determinant ();

	if (determinant > Math::SmallValue)
	{
		const float rcpDet = 1.0f / determinant;

		const float m00 = rcpDet * (
			m_values[1][1] * (m_values[2][2] * m_values[3][3] - m_values[2][3] * m_values[3][2]) -
			m_values[1][2] * (m_values[2][1] * m_values[3][3] - m_values[2][3] * m_values[3][1]) +
			m_values[1][3] * (m_values[2][1] * m_values[3][2] - m_values[2][2] * m_values[3][1]));
		const float m01 = -rcpDet * (
			m_values[0][1] * (m_values[2][2] * m_values[3][3] - m_values[2][3] * m_values[3][2]) -
			m_values[0][2] * (m_values[2][1] * m_values[3][3] - m_values[2][3] * m_values[3][1]) +
			m_values[0][3] * (m_values[2][1] * m_values[3][2] - m_values[2][2] * m_values[3][1]));
		const float m02 = rcpDet * (
			m_values[0][1] * (m_values[1][2] * m_values[3][3] - m_values[1][3] * m_values[3][2]) -
			m_values[0][2] * (m_values[1][1] * m_values[3][3] - m_values[1][3] * m_values[3][1]) +
			m_values[0][3] * (m_values[2][1] * m_values[3][2] - m_values[2][2] * m_values[3][1]));
		const float m03 = -rcpDet * (
			m_values[0][1] * (m_values[1][2] * m_values[2][3] - m_values[1][3] * m_values[2][2]) -
			m_values[0][2] * (m_values[1][1] * m_values[2][3] - m_values[1][3] * m_values[2][1]) +
			m_values[0][3] * (m_values[1][1] * m_values[2][2] - m_values[1][2] * m_values[2][1]));

		const float m10 = -rcpDet * (
			m_values[1][0] * (m_values[2][2] * m_values[3][3] - m_values[2][3] * m_values[3][2]) -
			m_values[1][2] * (m_values[2][0] * m_values[3][3] - m_values[2][3] * m_values[3][0]) +
			m_values[1][3] * (m_values[2][0] * m_values[3][2] - m_values[2][2] * m_values[3][0]));
		const float m11 = rcpDet * (
			m_values[0][0] * (m_values[2][2] * m_values[3][3] - m_values[2][3] * m_values[3][2]) -
			m_values[0][2] * (m_values[2][0] * m_values[3][3] - m_values[2][3] * m_values[3][0]) +
			m_values[0][3] * (m_values[2][0] * m_values[3][2] - m_values[2][2] * m_values[3][0]));
		const float m12 = -rcpDet * (
			m_values[0][0] * (m_values[1][2] * m_values[3][3] - m_values[1][3] * m_values[3][2]) -
			m_values[0][2] * (m_values[1][0] * m_values[3][3] - m_values[1][3] * m_values[3][0]) +
			m_values[0][3] * (m_values[1][0] * m_values[3][2] - m_values[1][2] * m_values[3][0]));
		const float m13 = rcpDet * (
			m_values[0][0] * (m_values[1][2] * m_values[2][3] - m_values[1][3] * m_values[2][2]) -
			m_values[0][2] * (m_values[1][0] * m_values[2][3] - m_values[1][3] * m_values[2][0]) +
			m_values[0][3] * (m_values[1][0] * m_values[2][2] - m_values[1][2] * m_values[2][0]));

		const float m20 = rcpDet * (
			m_values[1][0] * (m_values[2][1] * m_values[3][3] - m_values[2][3] * m_values[3][1]) -
			m_values[1][1] * (m_values[2][0] * m_values[3][3] - m_values[2][3] * m_values[3][0]) +
			m_values[1][3] * (m_values[2][0] * m_values[3][1] - m_values[2][1] * m_values[3][0]));
		const float m21 = -rcpDet * (
			m_values[0][0] * (m_values[2][1] * m_values[3][3] - m_values[2][3] * m_values[3][1]) -
			m_values[0][1] * (m_values[2][0] * m_values[3][3] - m_values[2][3] * m_values[3][0]) +
			m_values[0][3] * (m_values[2][0] * m_values[3][1] - m_values[2][1] * m_values[3][0]));
		const float m22 = rcpDet * (
			m_values[0][0] * (m_values[1][1] * m_values[3][3] - m_values[1][3] * m_values[3][1]) -
			m_values[0][1] * (m_values[1][0] * m_values[3][3] - m_values[1][3] * m_values[3][0]) +
			m_values[0][3] * (m_values[2][0] * m_values[3][1] - m_values[2][1] * m_values[3][0]));
		const float m23 = -rcpDet * (
			m_values[0][0] * (m_values[1][1] * m_values[2][3] - m_values[1][3] * m_values[2][1]) -
			m_values[0][1] * (m_values[1][0] * m_values[2][3] - m_values[1][3] * m_values[2][0]) +
			m_values[0][3] * (m_values[1][0] * m_values[2][1] - m_values[1][1] * m_values[2][0]));

		const float m30 = -rcpDet * (
			m_values[1][0] * (m_values[2][1] * m_values[3][2] - m_values[2][2] * m_values[3][1]) -
			m_values[1][1] * (m_values[2][0] * m_values[3][2] - m_values[2][2] * m_values[3][0]) +
			m_values[1][2] * (m_values[2][0] * m_values[3][1] - m_values[2][1] * m_values[3][0]));
		const float m31 = rcpDet * (
			m_values[0][0] * (m_values[2][1] * m_values[3][2] - m_values[2][2] * m_values[3][1]) -
			m_values[0][1] * (m_values[2][0] * m_values[3][2] - m_values[2][2] * m_values[3][0]) +
			m_values[0][2] * (m_values[2][0] * m_values[3][1] - m_values[2][1] * m_values[3][0]));
		const float m32 = -rcpDet * (
			m_values[0][0] * (m_values[1][1] * m_values[3][2] - m_values[1][2] * m_values[3][1]) -
			m_values[0][1] * (m_values[1][0] * m_values[3][2] - m_values[1][2] * m_values[3][0]) +
			m_values[0][2] * (m_values[1][0] * m_values[3][1] - m_values[1][1] * m_values[3][0]));
		const float m33 = rcpDet * (
			m_values[0][0] * (m_values[1][1] * m_values[2][2] - m_values[1][2] * m_values[2][1]) -
			m_values[0][1] * (m_values[1][0] * m_values[2][2] - m_values[1][2] * m_values[2][0]) +
			m_values[0][2] * (m_values[1][0] * m_values[2][1] - m_values[1][1] * m_values[2][0]));

		return Matrix4x4 (
			m00, m01, m02, m03,
			m10, m11, m12, m13,
			m20, m21, m22, m23,
			m30, m31, m32, m33
		);
	}

	return Matrix4x4::Zero;
}

Matrix4x4 Matrix4x4::Transposed () const
{
	return Matrix4x4 (
		m_values[0][0], m_values[1][0], m_values[2][0], m_values[3][0],
		m_values[0][1], m_values[1][1], m_values[2][1], m_values[3][1],
		m_values[0][2], m_values[1][2], m_values[2][2], m_values[3][2],
		m_values[0][3], m_values[1][3], m_values[2][3], m_values[3][3]
	);
}

Vector4 Matrix4x4::GetRow (unsigned int index) const
{
	if (index > 3)
	{
		return Vector4::Zero;
	}

	return Vector4 (m_values[index][0], m_values[index][1], m_values[index][2], m_values[index][3]);
}

Vector4 Matrix4x4::GetColumn (unsigned int index) const
{
	if (index > 3)
	{
		return Vector4::Zero;
	}

	return Vector4 (m_values[0][index], m_values[1][index], m_values[2][index], m_values[3][index]);
}

void Matrix4x4::SetRow (unsigned int index, Vector4 values)
{
	if (index > 3)
	{
		return;
	}

	m_values[index][0] = values.m_x;
	m_values[index][1] = values.m_y;
	m_values[index][2] = values.m_z;
	m_values[index][3] = values.m_w;
}

void Matrix4x4::SetColumn (unsigned int index, Vector4 values)
{
	if (index > 3)
	{
		return;
	}

	m_values[0][index] = values.m_x;
	m_values[1][index] = values.m_y;
	m_values[2][index] = values.m_z;
	m_values[3][index] = values.m_w;
}

Matrix4x4 Matrix4x4::Translate (Vector3 translation)
{
	return Matrix4x4 (
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		translation.m_x, translation.m_y, translation.m_z, 1.0f
	);
}

Matrix4x4 Matrix4x4::Rotate (Quaternion rotation)
{
	const float x = rotation.m_x;
	const float y = rotation.m_y;
	const float z = rotation.m_z;
	const float w = rotation.m_w;

	const float xx = 2.0f * x * x;
	const float yy = 2.0f * y * y;
	const float zz = 2.0f * z * z;

	const float xy = 2.0f * x * y;
	const float xz = 2.0f * x * z;
	const float xw = 2.0f * x * w;
	const float yz = 2.0f * y * z;
	const float yw = 2.0f * y * w;
	const float zw = 2.0f * z * w;

	return Matrix4x4 (
		1.0f - yy - zz, xy + zw, xz - yw, 0.0f,
		xy - zw, 1.0f - xx - zz, yz + xw, 0.0f,
		xz + yw, yz - xw, 1.0f - xx - yy, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	);
}

Matrix4x4 Matrix4x4::Scale (Vector3 scaliling)
{
	return Matrix4x4 (
		scaliling.m_x, 0.0f, 0.0f, 0.0f,
		0.0f, scaliling.m_y, 0.0f, 0.0f,
		0.0f, 0.0f, scaliling.m_z, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	);
}

Matrix4x4 Matrix4x4::ScaleRotateTranslate (Vector3 translation, Quaternion rotation, Vector3 scaliling)
{
	return Scale (scaliling) * Rotate (rotation) * Translate (translation);
}

Matrix4x4 Matrix4x4::LookAt (Vector3 cameraPosition, Vector3 cameraLookAtPosition, Vector3 cameraUpDirection)
{
	const Vector3 zAxis = (cameraLookAtPosition - cameraPosition).Normalized ();
	const Vector3 xAxis = Vector3::Cross (cameraUpDirection, zAxis).Normalized ();
	const Vector3 yAxis = Vector3::Cross (zAxis, xAxis);

	const float dotX = -Vector3::Dot (xAxis, cameraPosition);
	const float dotY = -Vector3::Dot (yAxis, cameraPosition);
	const float dotZ = -Vector3::Dot (zAxis, cameraPosition);

	return Matrix4x4 (
		xAxis.m_x, yAxis.m_x, zAxis.m_x, 0.0f,
		xAxis.m_y, yAxis.m_y, zAxis.m_y, 0.0f,
		xAxis.m_z, yAxis.m_z, zAxis.m_z, 0.0f,
		dotX, dotY, dotZ, 1.0f
	);
}

Matrix4x4 Matrix4x4::Perspective (float fov, float aspectRatio, float zNear, float zFar)
{
	const float fovHalfRadians = 0.5f * fov * Math::Deg2Rad;

	const float cosFov = Math::Cos (fovHalfRadians);
	const float sinFov = Math::Sin (fovHalfRadians);

	const float yScale = sinFov ? cosFov / sinFov : 1.0f;
	const float xScale = yScale / aspectRatio;
	const float zScale = (zFar - zNear) ? zFar / (zFar - zNear) : 1.0f;

	return Matrix4x4 (
		xScale, 0.0f, 0.0f, 0.0f,
		0.0f, yScale, 0.0f, 0.0f,
		0.0f, 0.0f, zScale, 1.0f,
		0.0f, 0.0f, -zNear * zScale, 0.0f
	);
}

Matrix4x4 Matrix4x4::Orthographic (float width, float height, float zNear, float zFar)
{
	const float xScale = width ? 2.0f / width : 1.0f;
	const float yScale = height ? 2.0f / height : 1.0f;
	const float zScale = (zFar - zNear) ? 1.0f / (zFar - zNear) : 1.0f;

	return Matrix4x4 (
		xScale, 0.0f, 0.0f, 0.0f,
		0.0f, yScale, 0.0f, 0.0f,
		0.0f, 0.0f, zScale, 0.0f,
		0.0f, 0.0f, -zNear * zScale, 1.0f
	);
}

Matrix4x4& Matrix4x4::operator=(const Matrix4x4& other)
{
	m_values[0][0] = other.m_values[0][0];
	m_values[0][1] = other.m_values[0][1];
	m_values[0][2] = other.m_values[0][2];
	m_values[0][3] = other.m_values[0][3];

	m_values[1][0] = other.m_values[1][0];
	m_values[1][1] = other.m_values[1][1];
	m_values[1][2] = other.m_values[1][2];
	m_values[1][3] = other.m_values[1][3];

	m_values[2][0] = other.m_values[2][0];
	m_values[2][1] = other.m_values[2][1];
	m_values[2][2] = other.m_values[2][2];
	m_values[2][3] = other.m_values[2][3];

	m_values[3][0] = other.m_values[3][0];
	m_values[3][1] = other.m_values[3][1];
	m_values[3][2] = other.m_values[3][2];
	m_values[3][3] = other.m_values[3][3];

	return *this;
}

Matrix4x4 Matrix4x4::operator+ (const Matrix4x4& other) const
{
	return Matrix4x4 (
		m_values[0][0] + other.m_values[0][0], m_values[0][1] + other.m_values[0][1], m_values[0][2] + other.m_values[0][2], m_values[0][3] + other.m_values[0][3],
		m_values[1][0] + other.m_values[1][0], m_values[1][1] + other.m_values[1][1], m_values[1][2] + other.m_values[1][2], m_values[1][3] + other.m_values[1][3],
		m_values[2][0] + other.m_values[2][0], m_values[2][1] + other.m_values[2][1], m_values[2][2] + other.m_values[2][2], m_values[2][3] + other.m_values[2][3],
		m_values[3][0] + other.m_values[3][0], m_values[3][1] + other.m_values[3][1], m_values[3][2] + other.m_values[3][2], m_values[3][3] + other.m_values[3][3]
	);
}

Matrix4x4 Matrix4x4::operator- (const Matrix4x4& other) const
{
	return Matrix4x4 (
		m_values[0][0] - other.m_values[0][0], m_values[0][1] - other.m_values[0][1], m_values[0][2] - other.m_values[0][2], m_values[0][3] - other.m_values[0][3],
		m_values[1][0] - other.m_values[1][0], m_values[1][1] - other.m_values[1][1], m_values[1][2] - other.m_values[1][2], m_values[1][3] - other.m_values[1][3],
		m_values[2][0] - other.m_values[2][0], m_values[2][1] - other.m_values[2][1], m_values[2][2] - other.m_values[2][2], m_values[2][3] - other.m_values[2][3],
		m_values[3][0] - other.m_values[3][0], m_values[3][1] - other.m_values[3][1], m_values[3][2] - other.m_values[3][2], m_values[3][3] - other.m_values[3][3]
	);
}

Matrix4x4 Matrix4x4::operator* (const Matrix4x4& other) const
{
	const float m00 = m_values[0][0] * other.m_values[0][0] + m_values[0][1] * other.m_values[1][0] + m_values[0][2] * other.m_values[2][0] + m_values[0][3] * other.m_values[3][0];
	const float m01 = m_values[0][0] * other.m_values[0][1] + m_values[0][1] * other.m_values[1][1] + m_values[0][2] * other.m_values[2][1] + m_values[0][3] * other.m_values[3][1];
	const float m02 = m_values[0][0] * other.m_values[0][2] + m_values[0][1] * other.m_values[1][2] + m_values[0][2] * other.m_values[2][2] + m_values[0][3] * other.m_values[3][2];
	const float m03 = m_values[0][0] * other.m_values[0][3] + m_values[0][1] * other.m_values[1][3] + m_values[0][2] * other.m_values[2][3] + m_values[0][3] * other.m_values[3][3];

	const float m10 = m_values[1][0] * other.m_values[0][0] + m_values[1][1] * other.m_values[1][0] + m_values[1][2] * other.m_values[2][0] + m_values[1][3] * other.m_values[3][0];
	const float m11 = m_values[1][0] * other.m_values[0][1] + m_values[1][1] * other.m_values[1][1] + m_values[1][2] * other.m_values[2][1] + m_values[1][3] * other.m_values[3][1];
	const float m12 = m_values[1][0] * other.m_values[0][2] + m_values[1][1] * other.m_values[1][2] + m_values[1][2] * other.m_values[2][2] + m_values[1][3] * other.m_values[3][2];
	const float m13 = m_values[1][0] * other.m_values[0][3] + m_values[1][1] * other.m_values[1][3] + m_values[1][2] * other.m_values[2][3] + m_values[1][3] * other.m_values[3][3];

	const float m20 = m_values[2][0] * other.m_values[0][0] + m_values[2][1] * other.m_values[1][0] + m_values[2][2] * other.m_values[2][0] + m_values[2][3] * other.m_values[3][0];
	const float m21 = m_values[2][0] * other.m_values[0][1] + m_values[2][1] * other.m_values[1][1] + m_values[2][2] * other.m_values[2][1] + m_values[2][3] * other.m_values[3][1];
	const float m22 = m_values[2][0] * other.m_values[0][2] + m_values[2][1] * other.m_values[1][2] + m_values[2][2] * other.m_values[2][2] + m_values[2][3] * other.m_values[3][2];
	const float m23 = m_values[2][0] * other.m_values[0][3] + m_values[2][1] * other.m_values[1][3] + m_values[2][2] * other.m_values[2][3] + m_values[2][3] * other.m_values[3][3];

	const float m30 = m_values[3][0] * other.m_values[0][0] + m_values[3][1] * other.m_values[1][0] + m_values[3][2] * other.m_values[2][0] + m_values[3][3] * other.m_values[3][0];
	const float m31 = m_values[3][0] * other.m_values[0][1] + m_values[3][1] * other.m_values[1][1] + m_values[3][2] * other.m_values[2][1] + m_values[3][3] * other.m_values[3][1];
	const float m32 = m_values[3][0] * other.m_values[0][2] + m_values[3][1] * other.m_values[1][2] + m_values[3][2] * other.m_values[2][2] + m_values[3][3] * other.m_values[3][2];
	const float m33 = m_values[3][0] * other.m_values[0][3] + m_values[3][1] * other.m_values[1][3] + m_values[3][2] * other.m_values[2][3] + m_values[3][3] * other.m_values[3][3];

	return Matrix4x4 (
		m00, m01, m02, m03,
		m10, m11, m12, m13,
		m20, m21, m22, m23,
		m30, m31, m32, m33
	);
}

Matrix4x4 Matrix4x4::operator* (const float scale) const
{
	return Matrix4x4 (
		m_values[0][0] * scale, m_values[0][1] * scale, m_values[0][2] * scale, m_values[0][3] * scale,
		m_values[1][0] * scale, m_values[1][1] * scale, m_values[1][2] * scale, m_values[1][3] * scale,
		m_values[2][0] * scale, m_values[2][1] * scale, m_values[2][2] * scale, m_values[2][3] * scale,
		m_values[3][0] * scale, m_values[3][1] * scale, m_values[3][2] * scale, m_values[3][3] * scale
	);
}

Matrix4x4 Matrix4x4::operator/ (const float scale) const
{
	return Matrix4x4 (
		m_values[0][0] / scale, m_values[0][1] / scale, m_values[0][2] / scale, m_values[0][3] / scale,
		m_values[1][0] / scale, m_values[1][1] / scale, m_values[1][2] / scale, m_values[1][3] / scale,
		m_values[2][0] / scale, m_values[2][1] / scale, m_values[2][2] / scale, m_values[2][3] / scale,
		m_values[3][0] / scale, m_values[3][1] / scale, m_values[3][2] / scale, m_values[3][3] / scale
	);
}

Matrix4x4& Matrix4x4::operator+= (const Matrix4x4& other)
{
	m_values[0][0] += other.m_values[0][0];
	m_values[0][1] += other.m_values[0][1];
	m_values[0][2] += other.m_values[0][2];
	m_values[0][3] += other.m_values[0][3];

	m_values[1][0] += other.m_values[1][0];
	m_values[1][1] += other.m_values[1][1];
	m_values[1][2] += other.m_values[1][2];
	m_values[1][3] += other.m_values[1][3];

	m_values[2][0] += other.m_values[2][0];
	m_values[2][1] += other.m_values[2][1];
	m_values[2][2] += other.m_values[2][2];
	m_values[2][3] += other.m_values[2][3];

	m_values[3][0] += other.m_values[3][0];
	m_values[3][1] += other.m_values[3][1];
	m_values[3][2] += other.m_values[3][2];
	m_values[3][3] += other.m_values[3][3];

	return *this;
}

Matrix4x4& Matrix4x4::operator-= (const Matrix4x4& other)
{
	m_values[0][0] -= other.m_values[0][0];
	m_values[0][1] -= other.m_values[0][1];
	m_values[0][2] -= other.m_values[0][2];
	m_values[0][3] -= other.m_values[0][3];

	m_values[1][0] -= other.m_values[1][0];
	m_values[1][1] -= other.m_values[1][1];
	m_values[1][2] -= other.m_values[1][2];
	m_values[1][3] -= other.m_values[1][3];

	m_values[2][0] -= other.m_values[2][0];
	m_values[2][1] -= other.m_values[2][1];
	m_values[2][2] -= other.m_values[2][2];
	m_values[2][3] -= other.m_values[2][3];

	m_values[3][0] -= other.m_values[3][0];
	m_values[3][1] -= other.m_values[3][1];
	m_values[3][2] -= other.m_values[3][2];
	m_values[3][3] -= other.m_values[3][3];

	return *this;
}

Matrix4x4& Matrix4x4::operator*= (const Matrix4x4& other)
{
	const float m00 = m_values[0][0] * other.m_values[0][0] + m_values[0][1] * other.m_values[1][0] + m_values[0][2] * other.m_values[2][0] + m_values[0][3] * other.m_values[3][0];
	const float m01 = m_values[0][0] * other.m_values[0][1] + m_values[0][1] * other.m_values[1][1] + m_values[0][2] * other.m_values[2][1] + m_values[0][3] * other.m_values[3][1];
	const float m02 = m_values[0][0] * other.m_values[0][2] + m_values[0][1] * other.m_values[1][2] + m_values[0][2] * other.m_values[2][2] + m_values[0][3] * other.m_values[3][2];
	const float m03 = m_values[0][0] * other.m_values[0][3] + m_values[0][1] * other.m_values[1][3] + m_values[0][2] * other.m_values[2][3] + m_values[0][3] * other.m_values[3][3];

	const float m10 = m_values[1][0] * other.m_values[0][0] + m_values[1][1] * other.m_values[1][0] + m_values[1][2] * other.m_values[2][0] + m_values[1][3] * other.m_values[3][0];
	const float m11 = m_values[1][0] * other.m_values[0][1] + m_values[1][1] * other.m_values[1][1] + m_values[1][2] * other.m_values[2][1] + m_values[1][3] * other.m_values[3][1];
	const float m12 = m_values[1][0] * other.m_values[0][2] + m_values[1][1] * other.m_values[1][2] + m_values[1][2] * other.m_values[2][2] + m_values[1][3] * other.m_values[3][2];
	const float m13 = m_values[1][0] * other.m_values[0][3] + m_values[1][1] * other.m_values[1][3] + m_values[1][2] * other.m_values[2][3] + m_values[1][3] * other.m_values[3][3];

	const float m20 = m_values[2][0] * other.m_values[0][0] + m_values[2][1] * other.m_values[1][0] + m_values[2][2] * other.m_values[2][0] + m_values[2][3] * other.m_values[3][0];
	const float m21 = m_values[2][0] * other.m_values[0][1] + m_values[2][1] * other.m_values[1][1] + m_values[2][2] * other.m_values[2][1] + m_values[2][3] * other.m_values[3][1];
	const float m22 = m_values[2][0] * other.m_values[0][2] + m_values[2][1] * other.m_values[1][2] + m_values[2][2] * other.m_values[2][2] + m_values[2][3] * other.m_values[3][2];
	const float m23 = m_values[2][0] * other.m_values[0][3] + m_values[2][1] * other.m_values[1][3] + m_values[2][2] * other.m_values[2][3] + m_values[2][3] * other.m_values[3][3];

	const float m30 = m_values[3][0] * other.m_values[0][0] + m_values[3][1] * other.m_values[1][0] + m_values[3][2] * other.m_values[2][0] + m_values[3][3] * other.m_values[3][0];
	const float m31 = m_values[3][0] * other.m_values[0][1] + m_values[3][1] * other.m_values[1][1] + m_values[3][2] * other.m_values[2][1] + m_values[3][3] * other.m_values[3][1];
	const float m32 = m_values[3][0] * other.m_values[0][2] + m_values[3][1] * other.m_values[1][2] + m_values[3][2] * other.m_values[2][2] + m_values[3][3] * other.m_values[3][2];
	const float m33 = m_values[3][0] * other.m_values[0][3] + m_values[3][1] * other.m_values[1][3] + m_values[3][2] * other.m_values[2][3] + m_values[3][3] * other.m_values[3][3];

	m_values[0][0] = m00;
	m_values[0][1] = m01;
	m_values[0][2] = m02;
	m_values[0][3] = m03;

	m_values[1][0] = m10;
	m_values[1][1] = m11;
	m_values[1][2] = m12;
	m_values[1][3] = m13;

	m_values[2][0] = m20;
	m_values[2][1] = m21;
	m_values[2][2] = m22;
	m_values[2][3] = m23;

	m_values[3][0] = m30;
	m_values[3][1] = m31;
	m_values[3][2] = m32;
	m_values[3][3] = m33;

	return *this;
}

Matrix4x4& Matrix4x4::operator*= (const float scale)
{
	m_values[0][0] *= scale;
	m_values[0][1] *= scale;
	m_values[0][2] *= scale;
	m_values[0][3] *= scale;

	m_values[1][0] *= scale;
	m_values[1][1] *= scale;
	m_values[1][2] *= scale;
	m_values[1][3] *= scale;

	m_values[2][0] *= scale;
	m_values[2][1] *= scale;
	m_values[2][2] *= scale;
	m_values[2][3] *= scale;

	m_values[3][0] *= scale;
	m_values[3][1] *= scale;
	m_values[3][2] *= scale;
	m_values[3][3] *= scale;

	return *this;
}

Matrix4x4& Matrix4x4::operator/= (const float scale)
{
	m_values[0][0] /= scale;
	m_values[0][1] /= scale;
	m_values[0][2] /= scale;
	m_values[0][3] /= scale;

	m_values[1][0] /= scale;
	m_values[1][1] /= scale;
	m_values[1][2] /= scale;
	m_values[1][3] /= scale;

	m_values[2][0] /= scale;
	m_values[2][1] /= scale;
	m_values[2][2] /= scale;
	m_values[2][3] /= scale;

	m_values[3][0] /= scale;
	m_values[3][1] /= scale;
	m_values[3][2] /= scale;
	m_values[3][3] /= scale;

	return *this;
}

bool Matrix4x4::operator==(const Matrix4x4& other) const
{
	return (m_values[0][0] == other.m_values[0][0]) && (m_values[0][1] == other.m_values[0][1]) && (m_values[0][2] == other.m_values[0][2]) && (m_values[0][3] == other.m_values[0][3]) &&
		(m_values[1][0] == other.m_values[1][0]) && (m_values[1][1] == other.m_values[1][1]) && (m_values[1][2] == other.m_values[1][2]) && (m_values[1][3] == other.m_values[1][3]) &&
		(m_values[2][0] == other.m_values[2][0]) && (m_values[2][1] == other.m_values[2][1]) && (m_values[2][2] == other.m_values[2][2]) && (m_values[2][3] == other.m_values[2][3]) &&
		(m_values[3][0] == other.m_values[3][0]) && (m_values[3][1] == other.m_values[3][1]) && (m_values[3][2] == other.m_values[3][2]) && (m_values[3][3] == other.m_values[3][3]);
}

bool Matrix4x4::operator!=(const Matrix4x4& other) const
{
	return (m_values[0][0] != other.m_values[0][0]) || (m_values[0][1] != other.m_values[0][1]) || (m_values[0][2] != other.m_values[0][2]) || (m_values[0][3] != other.m_values[0][3]) ||
		(m_values[1][0] != other.m_values[1][0]) || (m_values[1][1] != other.m_values[1][1]) || (m_values[1][2] != other.m_values[1][2]) || (m_values[1][3] != other.m_values[1][3]) ||
		(m_values[2][0] != other.m_values[2][0]) || (m_values[2][1] != other.m_values[2][1]) || (m_values[2][2] != other.m_values[2][2]) || (m_values[2][3] != other.m_values[2][3]) ||
		(m_values[3][0] != other.m_values[3][0]) || (m_values[3][1] != other.m_values[3][1]) || (m_values[3][2] != other.m_values[3][2]) || (m_values[3][3] != other.m_values[3][3]);
}

Matrix4x4 operator* (const float scale, const Matrix4x4& matrix)
{
	return Matrix4x4 (
		matrix.m_values[0][0] * scale, matrix.m_values[0][1] * scale, matrix.m_values[0][2] * scale, matrix.m_values[0][3] * scale,
		matrix.m_values[1][0] * scale, matrix.m_values[1][1] * scale, matrix.m_values[1][2] * scale, matrix.m_values[1][3] * scale,
		matrix.m_values[2][0] * scale, matrix.m_values[2][1] * scale, matrix.m_values[2][2] * scale, matrix.m_values[2][3] * scale,
		matrix.m_values[3][0] * scale, matrix.m_values[3][1] * scale, matrix.m_values[3][2] * scale, matrix.m_values[3][3] * scale
	);
}

Vector4 operator* (const Vector4& vector, const Matrix4x4& matrix)
{
	const float x = vector.m_x;
	const float y = vector.m_y;
	const float z = vector.m_z;
	const float w = vector.m_w;

	const float resultX = x * matrix.m_values[0][0] + y * matrix.m_values[1][0] + z * matrix.m_values[2][0] + w * matrix.m_values[3][0];
	const float resultY = x * matrix.m_values[0][1] + y * matrix.m_values[1][1] + z * matrix.m_values[2][1] + w * matrix.m_values[3][1];
	const float resultZ = x * matrix.m_values[0][2] + y * matrix.m_values[1][2] + z * matrix.m_values[2][2] + w * matrix.m_values[3][2];
	const float resultW = x * matrix.m_values[0][3] + y * matrix.m_values[1][3] + z * matrix.m_values[2][3] + w * matrix.m_values[3][3];

	return Vector4 (resultX, resultY, resultZ, resultW);
}