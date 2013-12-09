#include "Transform.h"
#include "GameObject.h"


void Transform::update()
{
	m_matScale=XMMatrixScaling(m_vecScale.x,m_vecScale.y,m_vecScale.z);
	m_matRotation=XMMatrixRotationRollPitchYaw(m_vecRotation.x,m_vecRotation.y,m_vecRotation.z);
	m_matTranslation=XMMatrixTranslation(m_vecPosition.x,m_vecPosition.y,m_vecPosition.z);

	m_matWorld=XMMatrixMultiply(m_matScale,m_matRotation);
	m_matWorld=XMMatrixMultiply(m_matWorld,m_matTranslation);
	//get parent
	
	if (m_pOwnerGameObject->getParent())
	{
		Transform parentTransform=m_pOwnerGameObject->getTransform();
		m_matWorld=XMMatrixMultiply(m_matWorld,parentTransform.getWorld());
	}
};
