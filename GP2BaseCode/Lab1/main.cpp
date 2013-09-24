//Are we on the windows platform, we could resolve this check
//if we go for a build system such a CMake
#ifdef WIN32
//if so include the header for windows applications
#include <Windows.h>
#endif

#include "../GameApplication/GameApplication.h"

//Check to see if we are on windows
#ifdef WIN32
int WINAPI WinMain(HINSTANCE hInstance, 
					HINSTANCE hPrevInstance, 
					LPSTR lpCmdLine,
					int nCmdShow)
#else
//we are on some other platform
int main(int argc, char **argv)
#endif
{
	//new Game Application
	CGameApplication *pApp = new CGameApplication();
	//check if it worked
	if (!pApp->init())
	{
		//if something with the init went wrong, delete ptr and set it to NULL
		if (pApp)
		{
			delete pApp;
			pApp = NULL;
			return 1;
		}
	}
	//if everything worked, run the Game Application
	pApp->run();
	//delete the ptr and set it to NULL
	if (pApp)
	{	
		delete pApp;
		pApp = NULL;
	}
	
	return 0;
}

