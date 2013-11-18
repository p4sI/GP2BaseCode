#include "CubeVisualComponent.h"
#include "../D3D10Renderer/Vertex.h"
#include "../D3D10Renderer/D3D10Renderer.h"

bool CubeVisualComponent::create(IRenderer * pRenderer)
{
	
	Vertex verts[]={
		{-0.5f,-0.5f,0.0f},
		{-0.5f,0.5f,0.0f},
		{0.5f,-0.5f,0.0f},
		{0.5f,0.5f,0.0f}
	};
	m_iNoVerts=4;

	int indices[]={0,1,2,1,3,2};
	m_iNoIndices=6;

	D3D10Renderer *pD3D10Renderer=static_cast<D3D10Renderer*>(pRenderer);

	m_pVertexBuffer=pD3D10Renderer->createVertexBuffer(m_iNoVerts,verts);
	m_pIndexBuffer=pD3D10Renderer->createIndexBuffer(m_iNoIndices,indices);
	

	return true;
}