#ifndef _NEHEWINDOW_H
#define _NEHEWINDOW_H

#include "iwindow.h"
#include "windows.h"
#include <GL/glew.h>


class IGame;

class NeHeWindow : public IWindow 
{
public:
	NeHeWindow( IGame * );

	virtual ~NeHeWindow();

	virtual void Run();

	LRESULT WndProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

private:
	virtual	bool Initialise();

	virtual void Update(const float dtSeconds);

	virtual void ShutDownWindow();

	virtual void WindowResize( const int newWidth, const int newHeight );

	void	ResizeGLWindow(const int width, const int height);	

	bool	SetGLPixelFormat(); 	

	void	OpenGLInit();

	virtual const int WindowHeight() const;

	virtual const int WindowWidth() const;


private:

	//	parameters for windows and openGL setup

	HWND		hwnd;			// handle to our window.
	HDC			ghdc;			// Device Context.
	HGLRC		ghrc;			// hardware RENDERING CONTEXT.
	RECT		gRect;			// rect object representing the window client area. 
	static const	int		COLOUR_DEPTH = 32;	//Colour depth

	int S_WIDTH;				// client area width and height.
	int S_HEIGHT;				//

	bool run_client_update_loop_;	// Game loop condition
	bool first;						// bool to denote whether this is the first frame being rendered
									// in which case it skips it.  This is to ensure the Delta
									// time is a sensible value by making it run a 'dry' frame. 

	GLuint	base;				// Base Display List For The Font Set
	
	double			CurrentTime ;
	double			OldTime ;	
	double			DeltaTime ;

	
};


#endif