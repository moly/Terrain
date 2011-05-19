#include <windows.h>
#include "iwindow.h"
#include "igame.h"
#include "gamefactory.h"
#include "iinput.h"

HINSTANCE g_app_instance;	// global Hinstance -> used by the nehe or any other windows window class.
IInput *g_input;			// global input  class pointer -> used when the game update is called and also in the application



// Entry point into the application
int WINAPI WinMain (HINSTANCE hInstance, 
					HINSTANCE hPrevInstance,
                    PSTR szCmdLine, 
					int nCmdShow)	
{
	g_app_instance = hInstance;

	//use the factory to create our objects.
	IGame * game = get_game();
	IWindow * window = get_game_window( game );
	g_input = get_game_input();

	window->Run();			// Run the application

	// Cleanup our objects.
	release_game_input(&g_input);
	release_game_window( &window );
	release_game( &game );

	return 0;
}