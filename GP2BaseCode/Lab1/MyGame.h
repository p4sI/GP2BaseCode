#pragma once

#include "../GameApplication/GameApplication.h"

class MyGame:public CGameApplication
{
public:
	MyGame(){};
	~MyGame(){};

	bool initGame();
};