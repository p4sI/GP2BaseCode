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
		m_pVertexLayout=NULL;
		m_iNoIndices=0;
		m_iNoVerts=0;
		m_Name="Visual";
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
		if (m_pVertexLayout)
		{
			m_pVertexLayout->Release();
		}
	};

	virtual bool create(IRenderer * pRenderer){
		return true;
	};

	bool createVertexLayout(IRenderer * pRenderer);

	ID3D10Buffer* getVertexBuffer(){return m_pVertexBuffer;};
	
	ID3D10Buffer* getIndexBuffer(){return m_pIndexBuffer;};

	ID3D10InputLayout* getVertexLayout(){return m_pVertexLayout;};
	
	int getNoIndices()
	{
		return m_iNoIndices;
	};

	int getNoVerts()
	{
		return m_iNoVerts;
	};
protected:
	ID3D10Buffer * m_pVertexBuffer;
	ID3D10Buffer * m_pIndexBuffer;
	ID3D10InputLayout* m_pVertexLayout;
	int m_iNoIndices;
	int m_iNoVerts;
};