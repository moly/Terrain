
#include <cassert>
#include "iwindow.h"

bool IWindow::Initialise()
{
	assert( game_ );
	return game_->Initialise();
}

void IWindow::Update(const float dtSeconds)
{
	assert( game_ );
	game_->Update( dtSeconds );
}

void IWindow::ShutDownWindow()
{
	assert( game_ );
	game_->ShutDown();
}

void IWindow::WindowResize( const int newWidth, const int newHeight )
{
	assert( game_ );
	game_->OnWindowResize( newWidth, newHeight );
}