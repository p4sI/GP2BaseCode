#include "GameObject.h"

void GameObject::addComponent(GameComponent* pComponent)
{
	//do we have a component?
	if (m_Components.find(pComponent->componentID())==m_Components.end())
	{
		//not found, so add
		m_Components[pComponent->componentID()]=pComponent;
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
			++iter;
		}
	}

	m_Components.clear();
}

void GameObject::update()
{
	for(ComponentTableIter iter=m_Components.begin();iter!=m_Components.end();++iter)
	{
		GameComponent *pCurrentComponent=(*iter).second;
		if (pCurrentComponent)
		{
			pCurrentComponent->update();
		}
	}
}