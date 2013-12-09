#pragma once

#include "GameComponent.h"


#include <windows.h>
#define _XM_NO_INTRINSICS_
#include <xnamath.h>

class Transform:public GameComponent
{
public:
	Transform()
	{
		m_vecPosition=XMFLOAT3(0.0f,0.0f,0.0f);
		m_vecRotation=XMFLOAT3(0.0f,0.0f,0.0f);
		m_vecScale=XMFLOAT3(1.0f,1.0f,1.0f);
		m_matScale=XMMatrixIdentity();
		m_matRotation=XMMatrixIdentity();
		m_matTranslation=XMMatrixIdentity();
		m_matWorld=XMMatrixIdentity();
		m_Name="Transform";
	};

	~Transform()
	{
	};

	void update();

	void setScale(float x,float y,float z)
	{
		m_vecScale.x=x;
		m_vecScale.y=y;
		m_vecScale.z=z;
	};

	XMFLOAT3& getScale()
	{
		return m_vecScale;
	};

	void setRotation(float x,float y,float z)
	{
		m_vecRotation.x=x;
		m_vecRotation.y=y;
		m_vecRotation.z=z;
	};

	XMFLOAT3& getRotation()
	{
		return m_vecRotation;
	};

	void setPosition(float x,float y,float z)
	{
		m_vecPosition.x=x;
		m_vecPosition.y=y;
		m_vecPosition.z=z;
	};

	XMFLOAT3& getPosition()
	{
		return m_vecPosition;
	};

	XMMATRIX& getWorld()
	{
		return m_matWorld;
	};
private:
	XMFLOAT3 m_vecPosition;
	XMFLOAT3 m_vecRotation;
	XMFLOAT3 m_vecScale;
	XMMATRIX m_matTranslation;
	XMMATRIX m_matRotation;
	XMMATRIX m_matScale;
	XMMATRIX m_matWorld;
};