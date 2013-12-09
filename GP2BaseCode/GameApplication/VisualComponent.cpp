#include "VisualComponent.h"
#include "Material.h"
#include "GameObject.h"
#include "../D3D10Renderer/D3D10Renderer.h"

//this should be done when the effect has been created
bool VisualComponent::createVertexLayout(IRenderer * pRenderer)
{
	Material *pMaterial=static_cast<Material*>(m_pOwnerGameObject->getComponent("Material"));
	if (pMaterial)
	{
		D3D10Renderer * pD3D10Renderer=static_cast<D3D10Renderer*>(pRenderer);
		m_pVertexLayout=pD3D10Renderer->createVertexLayout(pMaterial->getEffect());
		return true;
	}
	return false;
}


bool VisualComponent::createVertexBuffer(int size,Vertex *pVerts,IRenderer *pRenderer)
{
	D3D10Renderer * pD3D10Renderer=static_cast<D3D10Renderer*>(pRenderer);
	m_pVertexBuffer=pD3D10Renderer->createVertexBuffer(size,pVerts);
	m_iNoVerts=size;
	if (m_pVertexBuffer)
		return true;
	return false;
}

bool VisualComponent::createIndexBuffer(int size,int *pIndices,IRenderer *pRenderer)
{
	D3D10Renderer * pD3D10Renderer=static_cast<D3D10Renderer*>(pRenderer);
	m_pIndexBuffer=pD3D10Renderer->createIndexBuffer(size,pIndices);
	m_iNoIndices=size;
	if (m_pIndexBuffer)
		return true;
	return true;
}