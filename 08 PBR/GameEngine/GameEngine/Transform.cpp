#include "Transform.h"
#include "Matrix4x4.h"

Transform::Transform () : Component (),
	m_position (Vector3::Zero), m_rotation (Quaternion::Identity), m_scale (Vector3::One)
{
}

Transform::~Transform ()
{
}

void Transform::OnUpdate (float deltaTime)
{
}

void Transform::OnRender ()
{
}

void Transform::Translate (Vector3 translation)
{
	m_position += translation;
}

void Transform::Rotate (Quaternion rotation)
{
	m_rotation = m_rotation * rotation;
}

Vector3 Transform::Forward () const
{
	return m_rotation * Vector3::Forward;
}

Vector3 Transform::Right () const
{
	return m_rotation * Vector3::Right;
}

Vector3 Transform::Up () const
{
	return m_rotation * Vector3::Up;
}

Vector3 Transform::GetPosition () const
{
	return m_position;
}

void Transform::SetPosition (Vector3 position)
{
	m_position = position;
}

Quaternion Transform::GetRotation () const
{
	return m_rotation;
}

void Transform::SetRotation (Quaternion rotation)
{
	m_rotation = rotation;
}

Vector3 Transform::GetScale () const
{
	return m_scale;
}

void Transform::SetScale (Vector3 scale)
{
	m_scale = scale;
}
