#pragma once

class GameObject;

class Component
{
public:
	Component ();
	virtual ~Component () = 0;

	virtual void OnUpdate (float deltaTime) = 0;
	virtual void OnRender () = 0;

	// 현재 자신을 소유하고 있는 게임오브젝트를 리턴한다.
	GameObject& GetGameObject () const;

	// 자신을 소유하는 게임오브젝트를 설정한다.
	void SetGameObject (GameObject& gameObject);

private:
	GameObject* m_gameObject;
};

