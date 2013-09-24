#include "GameApplication.h"

// should really check to see if we are on a windows platform
#include "../Window/Win32Window.h";

//Constructor (called when a new instance of the GameApplication class is initialised)
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

//Deconstructor (called when deleted)
CGameApplication::~CGameApplication(void)
{
	//Delete things in reverse order
	if(m_pWindow)
	{
		delete m_pWindow;
		m_pWindow=NULL;
	}
}

//Init
//This initialises all subsystems
//If one initialisation fails (return is false), false will be returned
//otherwise true will be returned
bool CGameApplication::init()
{
	if(!parseConfigFile())
		return false;
	if(!initWindow())
		return false;
	if(!initGraphics())
		return false;
	if(!initInput())
		return false;
	if(!initGame())
		return false;
	return true;
}

//called to parse the config file
bool CGameApplication::parseConfigFile()
{
	return true;
}

//initInput - Initialises the input
bool CGameApplication::initInput()
{
	return true;
}

//initPhysics - Initialises the physics system
bool CGameApplication::initPhysics()
{
	return true;
}
	
//initGraphics - Initialises the graphic subsystem
bool CGameApplication::initGraphics()
{
	return true;
}


//initWindow - Initialises the window
bool CGameApplication::initWindow()
{
	//Create a Win32 Window
	m_pWindow = new CWin32Window();
	m_pWindow->init(m_GameOptionDesc.gameName,m_GameOptionDesc.width,
					m_GameOptionDesc.height,m_GameOptionDesc.fullscreen);

	return true;
}

//called to init the game elements
bool CGameApplication::initGame()
{
	return true;
}

//called to put the game in a loop (aka game loop)
void CGameApplication::run()
{
	//while the window is not closed
	while(m_pWindow->running())
	{
		//check for all window messages
		m_pWindow->checkForWindowMessages();

		//update
		update();

		//render
		render();
	}
}

//Render, called to draw one frame of the game
void CGameApplication::render()
{
}

//Update, called to update the game
void CGameApplication::update()
{
}