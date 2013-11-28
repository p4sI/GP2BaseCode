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
	pMaterial->loadEffect("Effects/Texture.fx",m_pRenderer);
	pMaterial->loadDiffuseTexture("Textures/Spotlight.jpg",m_pRenderer);

	pTestObj=new GameObject();
	pTestObj->setName("TestObject1");
	pTestObj->addComponent(pCube);
	pTestObj->addComponent(pMaterial);

	pCube->createVertexLayout(m_pRenderer);
	
	pTestObj->getTransform().setPosition(-0.5f,1.0f,1.0f);

	m_GameObjectList.push_back(pTestObj);

	CameraComponent *pCameraComp=new CameraComponent();
	pCameraComp->setLook(0.0f,0.0f,0.0f);
	pCameraComp->setFOV(m_GameOptionDesc.width/m_GameOptionDesc.height);
	//Create Camera
	GameObject *pCameraGO=new GameObject();
	pCameraGO->setName("MainCamera");
	pCameraGO->addComponent(pCameraComp);
	setMainCamera(pCameraComp);
	pCameraGO->getTransform().setPosition(0.0f,0.0f,-10.0f);

	m_GameObjectList.push_back(pCameraGO);

	return true;
}