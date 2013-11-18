#include "MyGame.h"

bool MyGame::initGame()
{
	CubeVisualComponent *pCube=new CubeVisualComponent();
	pCube->create(m_pRenderer);

	GameObject *pTestObj=new GameObject();
	pTestObj->setName("TestObject");
	pTestObj->addComponent(pCube);

	m_GameObjectList.push_back(pTestObj);

	return true;
}