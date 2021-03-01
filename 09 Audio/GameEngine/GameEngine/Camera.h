#pragma once

#include "Component.h"
#include "Matrix4x4.h"

enum class CameraMode
{
	Perspective,
	Orthographic
};

class Camera : public Component
{
public:
	Camera ();
	virtual ~Camera ();

	virtual void OnUpdate (float deltaTime) override;
	virtual void OnRender () override;

	Matrix4x4 GetViewMatrix () const;
	Matrix4x4 GetProjectionMatrix () const;

	CameraMode GetMode () const;
	void SetMode (CameraMode mode);

	float GetFieldOfView () const;
	void SetFieldOfView (float fieldOfView);

	float GetNear () const;
	void SetNear (float zNear);

	float GetFar () const;
	void SetFar (float zFar);

	static Camera* Main;

private:
	CameraMode m_mode;
	float m_fieldOfView;
	float m_orthographicSize;
	float m_near;
	float m_far;
};

