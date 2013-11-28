#include "CameraComponent.h"
#include "Transform.h"
#include "GameObject.h"

void CameraComponent::update()
{
	//Get the transform from the Parent Game Object
	if (m_pOwnerGameObject)
	{
		Transform t=m_pOwnerGameObject->getTransform();
		XMFLOAT3 position=t.getPosition();
		m_View=XMMatrixLookAtLH(XMLoadFloat3(&position),XMLoadFloat3(&m_LookAt),XMLoadFloat3(&m_Up));
		m_Projection=XMMatrixPerspectiveFovLH(m_FOV,m_AspectRatio,m_Near,m_Far);
	}
}