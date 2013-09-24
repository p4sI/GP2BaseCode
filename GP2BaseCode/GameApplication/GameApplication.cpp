#include "GameApplication.h"

// should really check tosee if we are on a windows platform
#include "../Window/Win32Window.h";

//Constructor
CGameApplication::CGameApplication(VOID)
{
	//Set to NULL
	m_pWindow=NULL;
	//Set the Window name to GP2
	m_GameOptionDesc.gameName=TEXT("GP2");
	//Window Height and Width
	m_GameOptionDesc.width=640;
	m_GameOptionDesc.height=480;
	//Fullscreen
	m_GameOptionDesc.fullscreen=false;
	//Config options
	m_ConfigFileName=TEXT("game.cfg");
}

//Deconstructor
CGameApplication::~CGameApplication(void)
{
	//Delete things in reverse order
	if(m_pWindow)
	{
		delete m_pWindow;
		m_pWindow=NULL;
	}
}

