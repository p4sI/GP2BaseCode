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
	}
	return false;
}