#include "CubeVisualComponent.h"
#include "../D3D10Renderer/Vertex.h"
#include "../D3D10Renderer/D3D10Renderer.h"

bool CubeVisualComponent::create(IRenderer * pRenderer)
{
	
	Vertex verts[]={
		{-1.0f,-1.0f,0.0f},
		{-1.0f,1.0f,0.0f},
		{1.0f,-1.0f,0.0f},
		{1.0f,1.0f,0.0f}
	};

	int indices[]={0,1,2,1,3,2};

	D3D10Renderer *pD3D10Renderer=static_cast<D3D10Renderer*>(pRenderer);

	m_pVertexBuffer=pD3D10Renderer->createVertexBuffer(4,verts);
	m_pIndexBuffer=pD3D10Renderer->createIndexBuffer(6,indices);
	

	return true;
}