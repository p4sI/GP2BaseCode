#pragma once

#include "GameComponent.h"

#include <windows.h>
#define _XM_NO_INTRINSICS_
#include <xnamath.h>

#include "D3D10.h"
#include "../Renderer/Renderer.h"

#include <string>

using namespace std;

class Material:public GameComponent
{
public:
	Material()
	{
		m_Name="Material";
		m_pEffect=NULL;
		m_pCurrentTechnique=NULL;
		m_Ambient=XMCOLOR(0.3f,0.3f,0.3f,1.0f);
		m_Diffuse=XMCOLOR(0.5f,0.5f,0.5f,1.0f);
		m_Specular=XMCOLOR(1.0f,1.0f,1.0f,1.0f);
	};

	~Material()
	{
		if (m_pEffect)
		{
			m_pEffect->Release();
			m_pEffect=NULL;
		}
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

	bool loadEffect(const string& filename,IRenderer * pRenderer);
	void switchTechnique(const string& name);

	ID3D10Effect * getEffect()
	{
		return m_pEffect;
	};

	ID3D10EffectTechnique * getCurrentTechnique()
	{
		return m_pCurrentTechnique;
	};
private:
	XMCOLOR m_Ambient;
	XMCOLOR m_Diffuse;
	XMCOLOR m_Specular;
	ID3D10Effect *m_pEffect;
	ID3D10EffectTechnique *m_pCurrentTechnique;
};