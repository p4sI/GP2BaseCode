#pragma once

#include <string>
#include <map>

#include "GameComponent.h"
#include "Transform.h"
using namespace std;

class GameObject
{
public:
	GameObject()
	{
		m_Transform.setOwner(this);
	};

	~GameObject()
	{
		clearComponents();
	};

	void setName(const string& name)
	{
		m_Name=name;
	};

	Transform& getTransform()

	{
		return m_Transform;
	};


	void addComponent(GameComponent* pComponent);
	GameComponent * getComponent(const GameComponent::goc_id_type &name)
	{
		return m_Components[name];
	};

	void clearComponents();
	void update();

private:
	Transform m_Transform;
	string m_Name;
	typedef map<const GameComponent::goc_id_type,GameComponent*> ComponentTable;
	typedef map<const GameComponent::goc_id_type,GameComponent*>::iterator ComponentTableIter;
	ComponentTable m_Components;
};