#include "Camera.h"
#include "GameObject.h"
#include "Transform.h"
#include "Vector3.h"
#include "D3D11.h"

Camera* Camera::Main = nullptr;

Camera::Camera () :
	m_mode (CameraMode::Perspective), m_fieldOfView (60.0f),
	m_orthographicSize (5.0f), m_near (0.1f), m_far (1000.0f)
{
	if (Main == nullptr)
	{
		Main = this;
	}
}

Camera::~Camera ()
{
}

void Camera::OnUpdate (float deltaTime)
{
}

void Camera::OnRender ()
{
}

Matrix4x4 Camera::GetViewMatrix () const
{
	auto& transform = *GetGameObject ().GetComponent<Transform> ();
	Vector3 position = transform.GetPosition ();
	Vector3 lookAt = position + transform.Forward ();
	Vector3 up = transform.Up ();

	return Matrix4x4::LookAt (position, lookAt, up);
}

Matrix4x4 Camera::GetProjectionMatrix () const
{
	float width = g_d3d11.GetRenderWidth ();
	float height = g_d3d11.GetRenderHeight ();
	float aspectRatio = height == 0.0f ? 1.0f : width / height;

	if (m_mode == CameraMode::Perspective)
	{
		return Matrix4x4::Perspective (m_fieldOfView, aspectRatio, m_near, m_far);
	}
	else
	{
		float width = m_orthographicSize * aspectRatio;
		float height = m_orthographicSize;

		return Matrix4x4::Orthographic (width, height, m_near, m_far);
	}
}

CameraMode Camera::GetMode () const
{
	return m_mode;
}

void Camera::SetMode (CameraMode projection)
{
	m_mode = projection;
}

float Camera::GetFieldOfView () const
{
	return m_fieldOfView;
}

void Camera::SetFieldOfView (float fieldOfView)
{
	m_fieldOfView = fieldOfView;
}

float Camera::GetNear () const
{
	return m_near;
}

void Camera::SetNear (float zNear)
{
	m_near = zNear;
}

float Camera::GetFar () const
{
	return m_far;
}

void Camera::SetFar (float zFar)
{
	m_far = zFar;
}