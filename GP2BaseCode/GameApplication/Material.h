#pragma once

#include "GameComponent.h"

#include <windows.h>
#define _XM_NO_INTRINSICS_
#include <xnamath.h>

class Material:public GameComponent
{
public:
	Material()
	{
	};

	~Material()
	{
	};


	const goc_id_type& componentID()
	{
		return "Material";
	};

	void setAmbient(float r,float g,float b,float a)
	{
		m_Ambient=XMCOLOR(r,g,b,a);
	};

	XMCOLOR& getAmbient()
	{
		return m_Ambient;
	};

	void setDiffuse(float r,float g,float b,float a)
	{
		m_Diffuse=XMCOLOR(r,g,b,a);
	};

	XMCOLOR& getDiffuse()
	{
		return m_Diffuse;
	};
	
	void setSpecular(float r,float g,float b,float a)
	{
		m_Specular=XMCOLOR(r,g,b,a);
	};

	XMCOLOR& getSpecular()
	{
		return m_Specular;
	};
private:
	XMCOLOR m_Ambient;
	XMCOLOR m_Diffuse;
	XMCOLOR m_Specular;
};