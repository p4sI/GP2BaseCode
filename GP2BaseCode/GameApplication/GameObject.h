#pragma once

#include <string>
#include <map>

#include "GameComponent.h"
#include "Transform.h"
using namespace std;

class GameObject
{
public:
	typedef map<const string, GameObject*> ChildrenGameObjects;
	typedef map<const string, GameObject*>::iterator ChildrenGameObjectsIter;
public:
	GameObject()
	{
		m_Transform.setOwner(this);
	};

	~GameObject()
	{
		clearComponents();
		clearChildren();
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
	void clearChildren();

	void update();

	void addChild(GameObject *pChild);
	void setParent(GameObject *pParent)
	{
		m_pParent=pParent;
	};

	GameObject * getParent()
	{
		return m_pParent;
	};

	const string& getName()
	{
		return m_Name;
	};


	ChildrenGameObjectsIter getFirstChild()
	{
		return m_Children.begin();
	};

	ChildrenGameObjectsIter getLastChild()
	{
		return m_Children.end();
	};
private:
	Transform m_Transform;
	string m_Name;
	typedef map<const GameComponent::goc_id_type,GameComponent*> ComponentTable;
	typedef map<const GameComponent::goc_id_type,GameComponent*>::iterator ComponentTableIter;


	ComponentTable m_Components;
	ChildrenGameObjects m_Children;
	GameObject * m_pParent;
};