#include "GameObject.h"

void GameObject::addComponent(GameComponent* pComponent)
{
	const GameComponent::goc_id_type id=pComponent->getName();
	//do we have a component?
	if (m_Components.find(id)==m_Components.end())
	{
		//not found, so add
		pComponent->setOwner(this);
		m_Components[id]=pComponent;
	}
	else
	{
		//do nothing!
	}
	
}

void GameObject::clearComponents()
{
	ComponentTableIter iter=m_Components.begin();
	while(iter!=m_Components.end())
	{
		if ((*iter).second)
		{
			delete (*iter).second;
			iter=m_Components.erase(iter);
		}
		else
		{
			iter++;
		}
	}

	m_Components.clear();
}

void GameObject::update()
{
	m_Transform.update();
	for(ComponentTableIter iter=m_Components.begin();iter!=m_Components.end();iter++)
	{
		GameComponent *pCurrentComponent=(*iter).second;
		if (pCurrentComponent)
		{
			pCurrentComponent->update();
		}
	}
}