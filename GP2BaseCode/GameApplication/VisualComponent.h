#pragma once

#include "GameComponent.h"
#include "../Renderer/Renderer.h"

#include <D3D10.h>
#include <D3DX10.h>

class VisualComponent:public GameComponent
{
public:
	VisualComponent()
	{
		m_pVertexBuffer=NULL;
		m_pIndexBuffer=NULL;
	};
	
	virtual ~VisualComponent()
	{
		if (m_pVertexBuffer)
		{
			m_pVertexBuffer->Release();
		}
		if (m_pIndexBuffer)
		{
			m_pIndexBuffer->Release();
		}
	};

	virtual const goc_id_type& componentID()
	{
		return "Visual";
	};

	virtual bool create(IRenderer * pRenderer){return true;};

	ID3D10Buffer* getVertexBuffer(){return m_pVertexBuffer;};
	ID3D10Buffer* getIndexBuffer(){return m_pIndexBuffer;};
protected:
	ID3D10Buffer * m_pVertexBuffer;
	ID3D10Buffer * m_pIndexBuffer;
};