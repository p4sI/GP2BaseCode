#pragma once

#include "Light.h"

class DirectionalLight:public Light
{
public:
	DirectionalLight(){
		m_LightDirection=XMFLOAT3(0.0f,0.0f,-1.0f);
		m_Name="DirectionalLight";
	};

	~DirectionalLight(){};


	void setDirection(float x,float y,float z)
	{
		m_LightDirection=XMFLOAT3(x,y,z);
	};

	XMFLOAT3& getDirection()
	{
		return m_LightDirection;
	};
private:
	XMFLOAT3 m_LightDirection;
};