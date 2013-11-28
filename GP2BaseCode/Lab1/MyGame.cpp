#include "MyGame.h"

bool MyGame::initGame()
{
	CubeVisualComponent *pCube=new CubeVisualComponent();
	pCube->create(m_pRenderer);
	
	Material *pMaterial=new Material();
	pMaterial->loadEffect("Effects/Transform.fx",m_pRenderer);

	GameObject *pTestObj=new GameObject();
	pTestObj->setName("TestObject");
	pTestObj->addComponent(pCube);
	pTestObj->addComponent(pMaterial);

	pCube->createVertexLayout(m_pRenderer);
	
	pTestObj->getTransform().setPosition(2.0f,1.0f,1.0f);

	m_GameObjectList.push_back(pTestObj);

	pCube=new CubeVisualComponent();
	pCube->create(m_pRenderer);
	
	pMaterial=new Material();
	pMaterial->loadEffect("Effects/Transform.fx",m_pRenderer);

	pTestObj=new GameObject();
	pTestObj->setName("TestObject1");
	pTestObj->addComponent(pCube);
	pTestObj->addComponent(pMaterial);

	pCube->createVertexLayout(m_pRenderer);
	
	pTestObj->getTransform().setPosition(-0.5f,1.0f,1.0f);

	m_GameObjectList.push_back(pTestObj);

	CameraComponent *pCameraComp=new CameraComponent();
	pCameraComp->setLook(0.0f,0.0f,0.0f);
	//Create Camera
	GameObject *pCameraGO=new GameObject();
	pCameraGO->setName("MainCamera");
	return true;
}