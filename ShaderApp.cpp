#include <GL/glew.h>
#include <cassert>
#include "NeHeWindow.h"
#include "directinput.h"
#include "camera.h"
#include "terrain.h"
#include "light.h"
#include <iostream>
#include <time.h>
#include "bmp.h"		
#include <GL/glut.h>  //always include last.  can throw up compile errors otherwise

extern IInput * g_input;


// The 'Game'  Class
class ShaderApp : public IGame 
{
public:
	virtual	bool Initialise();

	virtual void Update(const float dtSeconds);

	virtual void ShutDown();

	virtual void OnWindowResize( const int newWidth, const int newHeight );

	void HandleInput(const float dtSeconds);

private:

public:

private:
	Camera* camera;
	Light* light;
	Terrain* terrain;

};


//Functions for the Object Factory

IWindow* get_game_window( IGame* game )
{
	return new NeHeWindow( game );
}

void release_game_window( IWindow** ppWindow )
{
	IWindow * pwindow = *ppWindow;
	delete pwindow;
	*ppWindow = 0;
}

IGame* get_game()
{
	return new ShaderApp();
}

void release_game( IGame** ppGame )
{
	IGame * pGame = * ppGame;
	delete pGame;
	*ppGame = 0;
}

IInput* get_game_input()
{
	return new DirectInput();
}

void release_game_input( IInput** ppInput )
{
	IInput * pInput = *ppInput;
	delete pInput;
	*ppInput = 0;
}


///////////////////////////////////////////////////////////////////////////////////

bool ShaderApp::Initialise()
{
	srand(time(NULL));

	// set GL rendering State
	// seperate into function if it gets too much
	glEnable(GL_CULL_FACE);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	camera = new Camera();
	camera->PositionCamera(0, -15, 126,   25, 0);
	camera->movementSpeed = 15;

	terrain = new Terrain(80, 80);

	light = new Light(1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 190.0f, 50.0f);
	light->Enable();

	return true;
}

void ShaderApp::Update(const float dtSeconds)
{	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);// Clear The Screen And The Depth Buffer
	
	glLoadIdentity();
	HandleInput(dtSeconds);
	terrain->Render();
	//glutSolidTeapot(2);
	
}

void ShaderApp::HandleInput(const float dtSeconds)
{
	float strafe = 0.0f, advance = 0.0f, upDown = 0.0f, xRotation = 0.0f, yRotation = 0.0f;

	// left and right
	if(g_input->DigitalControlMap[DIK_LEFT] == true)
		strafe += camera->movementSpeed * dtSeconds;
	if(g_input->DigitalControlMap[DIK_RIGHT] == true)
		strafe -= camera->movementSpeed * dtSeconds;

	// forwards and backwards
	if(g_input->DigitalControlMap[DIK_UP] == true)
		advance -= camera->movementSpeed * dtSeconds;
	if(g_input->DigitalControlMap[DIK_DOWN] == true)
		advance += camera->movementSpeed * dtSeconds;

	// rotation
	if(g_input->DigitalControlMap[DIK_A] == true)
		yRotation -= camera->movementSpeed * dtSeconds;
	if(g_input->DigitalControlMap[DIK_D] == true)
		yRotation += camera->movementSpeed * dtSeconds;

	// more rotation
	if(g_input->DigitalControlMap[DIK_W] == true)
		xRotation -= camera->movementSpeed * dtSeconds;
	if(g_input->DigitalControlMap[DIK_S] == true)
		xRotation += camera->movementSpeed * dtSeconds;

	// manipulate the terrain
	if(g_input->DigitalControlMap[DIK_N] == true)
		terrain->Regenerate();
	if(g_input->DigitalControlMap[DIK_1] == true)
	{
		camera->PositionCamera(0, -15, 126,   25, 0);
		camera->collisionOn = false;
	}
	if(g_input->DigitalControlMap[DIK_2] == true)
	{
		camera->PositionCamera(0, 0, 0, 0, 0);
		camera->collisionOn = true;
	}

	if(GetAsyncKeyState(VK_LBUTTON))
		advance -= camera->movementSpeed * dtSeconds;

	if(GetAsyncKeyState(VK_RBUTTON))
		advance += camera->movementSpeed * dtSeconds;

	POINT mouse;
	GetCursorPos(&mouse);
	ScreenToClient(GetActiveWindow(), &mouse);
	
	RECT windowRect;
	GetWindowRect(GetActiveWindow(), &windowRect);
	int centreX = (windowRect.right - windowRect.left) / 2;
	int centreY = (windowRect.bottom - windowRect.top) / 2;

	camera->Update(strafe, advance, upDown, (float)(mouse.y - centreY) / 380.0f, (float)(mouse.x - centreX) / 380.0f, -terrain->VertexAt(camera->GetX() + 32, camera->GetZ() + 32).y);
}

void ShaderApp::ShutDown()
{	
	delete terrain;
	terrain = 0;
}

void ShaderApp::OnWindowResize( const int newWidth, const int newHeight )
{

}