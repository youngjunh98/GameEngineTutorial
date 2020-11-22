#include "GameObject.h"
#include "Transform.h"

GameObject::GameObject (const std::string& name) :
	m_name (name)
{
	AddComponent<Transform> ();
}

GameObject::~GameObject ()
{
}
