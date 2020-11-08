#pragma once

#include <string>
#include <vector>
#include <memory>
#include <utility>
#include <type_traits>

#include "Component.h"

class GameObject
{
public:
	GameObject (const std::string& name = "");
	virtual ~GameObject ();

	template<typename T>
	void AddComponent ()
	{
		// 템플릿 타입이 Component를 상속 받는지 확인한다.
		static_assert (std::is_base_of<Component, T>::value, "Invalid component type!");

		// 컴포넌트를 생성하고, 컴포넌트를 소유하는 게임 오브젝트를 자기 자신으로 설정한다.
		auto component = std::make_unique<T> ();
		component->SetGameObject (*this);

		// 컴포넌트 리스트에 추가한다.
		m_components.push_back (std::move (component));
	}

	template<typename T>
	T* GetComponent () const
	{
		// 템플릿 타입이 Component를 상속 받는지 확인한다.
		static_assert (std::is_base_of<Component, T>::value, "Invalid component type!");

		for (const auto& component : m_components)
		{
			// 컴포넌트를 찾으려는 타입으로 캐스팅한다.
			T* casted = dynamic_cast<T*> (component.get ());

			// 캐스팅이 성공하면 리턴한다.
			if (casted != nullptr)
			{
				return casted;
			}
		}

		return nullptr;
	}

	template<typename T>
	std::vector<T*> GetComponents () const
	{
		// 템플릿 타입이 Component를 상속 받는지 확인한다.
		static_assert (std::is_base_of<Component, T>::value, "Invalid component type!");

		std::vector<T*> found;

		for (const auto& component : m_components)
		{
			// 컴포넌트를 찾으려는 타입으로 캐스팅한다.
			T* casted = dynamic_cast<T*> (component.get ());

			// 캐스팅이 성공하면 찾은 컴포넌트 리스트에 추가한다.
			if (casted != nullptr)
			{
				found.push_back (casted);
			}
		}

		return found;
	}

	template<>
	std::vector<Component*> GetComponents<Component> () const
	{
		// GetComponents의 템플릿 타입이 Component일 때 사용하는 특수 버전.
		// 따라서 모든 컴포넌트를 리턴한다.
		std::vector<Component*> all;

		for (const auto& component : m_components)
		{
			all.push_back (component.get ());
		}

		return all;
	}

private:
	std::string m_name;
	std::vector<std::unique_ptr<Component>> m_components;
};

