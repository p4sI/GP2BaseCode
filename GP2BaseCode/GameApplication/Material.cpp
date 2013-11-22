#include "Material.h"

#include "../D3D10Renderer/D3D10Renderer.h"

bool Material::loadEffect(const string& filename,IRenderer * pRenderer)
{
	D3D10Renderer *pD3D10Renderer=static_cast<D3D10Renderer*>(pRenderer);

	m_pEffect=pD3D10Renderer->loadEffectFromFile(filename.c_str());
	if (!m_pEffect)
	{
		return false;
	}
	m_pCurrentTechnique=m_pEffect->GetTechniqueByIndex(0);
	return true;
}

void Material::switchTechnique(const string& name)
{
	if (m_pEffect)
	{
		m_pCurrentTechnique=m_pEffect->GetTechniqueByName(name.c_str());
	}
}