#pragma once

#include "GameComponent.h"

#include <windows.h>
#define _XM_NO_INTRINSICS_
#include <xnamath.h>

class CameraComponent:public GameComponent
{
public:
	CameraComponent()
	{
		m_Name="Camera";
		m_LookAt=XMFLOAT3(0.0f,0.0f,0.0f);
		m_Up=XMFLOAT3(0.0f,1.0f,0.0f);
		m_View=XMMatrixIdentity();
		m_Projection=XMMatrixIdentity();
		m_FOV=XM_PI/4;
		m_AspectRatio=800.0f/640.0f;
		m_Near=0.1f;
		m_Far=1000.0f;
	};

	~CameraComponent()
	{
	}


	void update();

	void setLook(float x,float y,float z)
	{
		m_LookAt=XMFLOAT3(x,y,z);
	};

	void setUp(float x,float y,float z)
	{
		m_Up=XMFLOAT3(x,y,z);
	};

	void setFOV(float fov)
	{
		m_FOV=fov;
	};

	void setAspectRatio(float aspectRatio)
	{
		m_AspectRatio=aspectRatio;
	};

	void setNearClip(float nearClip)
	{
		m_Near=nearClip;
	};

	void setFarClip(float farClip)
	{
		m_Far=farClip;
	};

	XMFLOAT3& getLookAt()
	{
		return m_LookAt;
	};

	XMFLOAT3& getUp()
	{
		return m_Up;
	};

	float getFOV()
	{
		return m_FOV;
	};

	float getAspectRatio()
	{
		return m_AspectRatio;
	};

	float getNear()
	{
		return m_Near;
	};

	float getFar()
	{
		return m_Far;
	};

	XMMATRIX& getView()
	{
		return m_View;
	};

	XMMATRIX& getProjection()
	{
		return m_Projection;
	};
private:
	XMFLOAT3 m_LookAt;
	XMFLOAT3 m_Up;
	float m_FOV;
	float m_AspectRatio;
	float m_Near;
	float m_Far;

	XMMATRIX m_View;
	XMMATRIX m_Projection;
};
