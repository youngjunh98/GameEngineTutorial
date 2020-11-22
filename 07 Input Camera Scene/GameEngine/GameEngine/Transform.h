#pragma once

#include "Component.h"
#include "Vector3.h"
#include "Quaternion.h"

class Transform : public Component
{
public:
	Transform ();
	virtual ~Transform ();

	virtual void OnUpdate (float deltaTime) override;
	virtual void OnRender () override;

	void Translate (Vector3 translation);
	void Rotate (Quaternion rotation);

	Vector3 Forward () const;
	Vector3 Right () const;
	Vector3 Up () const;

	Vector3 GetPosition () const;
	void SetPosition (Vector3 position);

	Quaternion GetRotation () const;
	void SetRotation (Quaternion rotation);

	Vector3 GetScale () const;
	void SetScale (Vector3 scale);
private:
	Vector3 m_position;
	Quaternion m_rotation;
	Vector3 m_scale;
};
