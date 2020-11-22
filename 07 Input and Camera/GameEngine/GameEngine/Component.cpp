#include "Component.h"
#include "GameObject.h"

Component::Component () :
	m_gameObject (nullptr)
{
}

Component::~Component ()
{
	m_gameObject = nullptr;
}

GameObject& Component::GetGameObject () const
{
	return *m_gameObject;
}

void Component::SetGameObject (GameObject& gameObject)
{
	m_gameObject = &gameObject;
}
