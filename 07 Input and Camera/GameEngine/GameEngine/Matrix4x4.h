#pragma once

class Vector3;
class Vector4;
class Quaternion;

class Matrix4x4
{
public:
	Matrix4x4 (
		float m00 = 0.0f, float m01 = 0.0f, float m02 = 0.0f, float m03 = 0.0f,
		float m10 = 0.0f, float m11 = 0.0f, float m12 = 0.0f, float m13 = 0.0f,
		float m20 = 0.0f, float m21 = 0.0f, float m22 = 0.0f, float m23 = 0.0f,
		float m30 = 0.0f, float m31 = 0.0f, float m32 = 0.0f, float m33 = 0.0f);
	Matrix4x4 (const Matrix4x4& other);
	~Matrix4x4 ();

	float Determinant () const;
	Matrix4x4 Inversed () const;
	Matrix4x4 Transposed () const;

	Vector4 GetRow (unsigned int index) const;
	Vector4 GetColumn (unsigned int index) const;

	void SetRow (unsigned int index, Vector4 values);
	void SetColumn (unsigned int index, Vector4 values);

	static Matrix4x4 Translate (Vector3 translation);
	static Matrix4x4 Rotate (Quaternion rotation);
	static Matrix4x4 Scale (Vector3 scaliling);
	static Matrix4x4 ScaleRotateTranslate (Vector3 translation, Quaternion rotation, Vector3 scaliling);

	static Matrix4x4 LookAt (Vector3 cameraPosition, Vector3 cameraLookAtPosition, Vector3 cameraUpDirection);
	static Matrix4x4 Perspective (float fov, float aspectRatio, float zNear, float zFar);
	static Matrix4x4 Orthographic (float width, float height, float zNear, float zFar);

	Matrix4x4& operator= (const Matrix4x4& other);
	Matrix4x4 operator+ (const Matrix4x4& other) const;
	Matrix4x4 operator- (const Matrix4x4& other) const;
	Matrix4x4 operator* (const Matrix4x4& other) const;
	Matrix4x4 operator* (const float scale) const;
	Matrix4x4 operator/ (const float scale) const;
	Matrix4x4& operator+= (const Matrix4x4& other);
	Matrix4x4& operator-= (const Matrix4x4& other);
	Matrix4x4& operator*= (const Matrix4x4& other);
	Matrix4x4& operator*= (const float scale);
	Matrix4x4& operator/= (const float scale);
	bool operator== (const Matrix4x4& other) const;
	bool operator!= (const Matrix4x4& other) const;

	friend Matrix4x4 operator* (const float scale, const Matrix4x4& matrix);
	friend Vector4 operator* (const Vector4& vector, const Matrix4x4& matrix);

public:
	static const Matrix4x4 Zero;
	static const Matrix4x4 Identity;

private:
	// Memory Layout
	// [0][0] [0][1] [0][2] [0][3]
	// [1][0] [1][1] [1][2] [1][3]
	// [2][0] [2][1] [2][2] [2][3]
	// [3][0] [3][1] [3][2] [3][3]
	float m_values[4][4];
};