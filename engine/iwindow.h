
#ifndef _IWINDOW_H
#define _IWINDOW_H

#include "igame.h"

//-----------------------------------------------------------------------------
// Cross platform game window.
//-----------------------------------------------------------------------------
class IWindow 
{
public:
	IWindow( IGame* game ) : game_( game ) {}

	virtual ~IWindow()	{ game_ = 0; }

	virtual void Run()			=	0;

protected:

	virtual	bool Initialise()	=	0;

	virtual void Update(const float dtSeconds) = 0;

	virtual void ShutDownWindow()		=	0;

	virtual void WindowResize( const int newWidth, const int newHeight ) = 0;

	virtual const int WindowHeight() const	=	0;

	virtual const int WindowWidth() const = 0;

	

private:
	IGame*	game_;
};


#endif