#include "VisualComponent.h"

class CubeVisualComponent:public VisualComponent
{
	CubeVisualComponent(){};
	~CubeVisualComponent(){};

	const goc_id_type& componentID()
	{
		return "CubeVisual";
	};

	bool create(IRenderer * pRenderer);
};