#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>

// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

#include <boost/program_options.hpp>

#include <DescentLogic/src/DescentFramework.h>

// Disabled due to link problems with boost right now
//#include "CommandLineOptions.h"

int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow) {

	//std::vector < std::string > args = boost::program_options::split_winmain(lpCmdLine);
	SDLInterfaceInitData sdlInitData;

	sdlInitData.Fullscreen = true;
	/*problem when linking the boost.programoptions library on windows
	 * if (commandline::handleCommandLine(sdlInitData, args) == false)
	 return 0;*/
	{ //scope of the Framework pointer
		auto f = std::make_unique < DescentFramework > (false, // level editor
		false, // demo mode
		false, // muted
		true, // forward scroll
		true //shop fps
				);

		f->initRenderEngine(sdlInitData);
		f->execute();
	}

	logging::Info() << "Calling SDL_Quit";
	SDL_Quit();

	return 0;
}
