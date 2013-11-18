//Components needed
// Transform - Done
// Material -
// Visual - 
// Camera -
// Directional Light - 

#pragma once

#include <string>

class GameObject;

class GameComponent
{
public:
	typedef std::string goc_id_type;

	GameComponent(){
		m_pOwnerGameObject=NULL;
	};

	virtual ~GameComponent()=0{};

	virtual const goc_id_type& componentID()=0;
	
	virtual void update(){};

	void setOwner(GameObject * pGO)
	{
		m_pOwnerGameObject=pGO;
	};

	GameObject * getOwner()
	{
		return m_pOwnerGameObject;
	}

private:
	GameObject *m_pOwnerGameObject;
};