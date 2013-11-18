#pragma once

#include "GameComponent.h"
#include "../Renderer/Renderer.h"

#include <D3D10.h>
#include <D3DX10.h>

class EffectComponent:public GameComponent
{
public:
	EffectComponent()
	{
		m_Name="Effect";
		m_pEffect=NULL;
		m_pTechnique=NULL;
	};
	
	~EffectComponent()
	{
		if (m_pEffect)
		{
			m_pEffect->Release();
			m_pEffect=NULL;
		}
	};


	ID3D10Effect * getEffect(){return m_pEffect;};
	ID3D10EffectTechnique * getTechnique(){return m_pTechnique;};
private:
	ID3D10Effect * m_pEffect;
	ID3D10EffectTechnique * m_pTechnique;
};