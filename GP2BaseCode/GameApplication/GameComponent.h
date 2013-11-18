//Components needed
// Transform - Done
// Material - Doneish!
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
		m_Name="BaseComponent";
	};

	virtual ~GameComponent()=0{};

	virtual const goc_id_type& getName(){ return m_Name;};
	
	virtual void update(){};

	void setOwner(GameObject * pGO)
	{
		m_pOwnerGameObject=pGO;
	};

	GameObject * getOwner()
	{
		return m_pOwnerGameObject;
	}

protected:
	GameObject *m_pOwnerGameObject;
	goc_id_type m_Name;
};