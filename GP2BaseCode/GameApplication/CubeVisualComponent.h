#include "VisualComponent.h"

class CubeVisualComponent:public VisualComponent
{
public:
	CubeVisualComponent(){};
	~CubeVisualComponent(){};

	bool create(IRenderer * pRenderer);
};