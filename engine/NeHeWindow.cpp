
#include "NeHeWindow.h"
#include <windows.h>
#include <mmsystem.h>
#include <GL/glew.h>
#include <cassert>
#include "IInput.h"

extern HINSTANCE g_app_instance;
extern IInput * g_input;


namespace 
{
	///////////////////////////////////////////////////////////////////////////////
    // WndProc()
    // Static msg handler which passes messages to the application class.
    ///////////////////////////////////////////////////////////////////////////////
    LRESULT CALLBACK WndProcShared( HWND hWnd, UINT msg, WPARAM wPrm, LPARAM lParam )
    {
        LRESULT result = 0;

        LONG_PTR p = ::GetWindowLongPtr(hWnd,0);
		
		// Since wndproc cannot be a method, we must get the message here
		// Identify the object which owns the actual method handler for 
		// that object and send it there.  
        if( p )
        {
            result = ((NeHeWindow*)(p))->WndProc( hWnd, msg, wPrm, lParam );
        }
        else
        {
            if( msg == WM_CREATE )
            {
                CREATESTRUCT* cs = (CREATESTRUCT*)lParam;
				::SetWindowLongPtr( hWnd, 0, (LONG_PTR)(cs->lpCreateParams) );
            }
            else
            {
				result = ::DefWindowProc(hWnd,msg,wPrm,lParam);
            }
        }

        return result;
    }
}

void NeHeWindow::Run()
{	
	///////////////////////////////////////////////////////////////////////////////
    // Run()
    // Entry point for our window where the message pump and the "main" loop exists.
    ///////////////////////////////////////////////////////////////////////////////
    MSG         msg;	
	
	// show the window 
	ShowWindow(hwnd, SW_SHOWNORMAL);	
	UpdateWindow(hwnd);			 
	SetFocus(hwnd);		
	
	run_client_update_loop_ = true;
	
	// main window loop
	while (run_client_update_loop_)					
    {				

		if (PeekMessage(&msg,NULL,0,0,PM_REMOVE))
		{

		    if (msg.message==WM_QUIT)
				break;

			TranslateMessage (&msg);	

			DispatchMessage (&msg);
		}

		else
		{		
			// calculate delta time and update

			timeBeginPeriod(1);
			OldTime = CurrentTime;
			CurrentTime = timeGetTime();
			DeltaTime = (CurrentTime - OldTime)*0.001;
			
			// ensure that first time round, the frame is not drawn.  This ensures delta time is accurate.
			if(!first)
			{
				Update(DeltaTime);	
			}
			else
			{
				first = false;
			}
		}

    }// End of main window loop
	
}

void NeHeWindow::Update(const float dtSeconds)
{	
	///////////////////////////////////////////////////////////////////////////////
    // Update()
    // Hits this every frame, the main game loop is updated here
    ///////////////////////////////////////////////////////////////////////////////

	// Updates game.
	g_input->Update();
	IWindow::Update( dtSeconds );
	
	// effectively the end of frame,  swap the frame buffers. 
	SwapBuffers(ghdc);// Swap the frame buffers.
}


 LRESULT NeHeWindow::WndProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{	
	///////////////////////////////////////////////////////////////////////////////////////
    // WndProc()
    // The objects windproc function. Message is passed in from the static wndproc function
    ///////////////////////////////////////////////////////////////////////////////////////
    switch (message)											
    {														

		case WM_SIZE:
			GetClientRect(hwnd, &gRect);
			WindowResize( (gRect.right-gRect.left), (gRect.bottom-gRect.top));
			break;	

		case WM_CLOSE:
            run_client_update_loop_ = false;
			ShutDownWindow();
            return 0;
	}													

	return DefWindowProc (hwnd, message, wParam, lParam);		
															
}
 
NeHeWindow::NeHeWindow( IGame * pgame ) : IWindow( pgame )
{
	///////////////////////////////////////////////////////////////////////////////
    // Constructor
    // Calls Initialisation for Window and OpenGL
    ///////////////////////////////////////////////////////////////////////////////

	hwnd	= NULL;			
	ghdc	= NULL;			
	ghrc	= NULL;				

	S_WIDTH		= 800;				// client area width and height.
	S_HEIGHT	= 600;		

	CurrentTime = timeGetTime();
	OldTime		= timeGetTime();	
	DeltaTime	= 0.001;

	first		= true;				// this will be our first time round the game loop

	Initialise();
}


void	NeHeWindow::OpenGLInit()
{
	///////////////////////////////////////////////////////////////////////////////
    // OpenGLInit()
    // Initialist and setup- opengl Rendering and GLEW initialisation
    ///////////////////////////////////////////////////////////////////////////////
	

	ghdc = GetDC(hwnd);

	bool pixformatsuccess = SetGLPixelFormat();
	assert(pixformatsuccess);

	ghrc = wglCreateContext(ghdc);		// creates  rendering context from  hdc
	assert(ghrc);

    wglMakeCurrent(ghdc, ghrc);			// Use this HRC.

	ResizeGLWindow(S_WIDTH, S_HEIGHT);	// Setup the Screen

	GLenum err = glewInit();

	if (GLEW_OK != err)
	{

		
	}

	glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);				// Black Background
	glClearDepth(1.0f);									// Depth Buffer Setup
	glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
	glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations
	glEnable(GL_LIGHTING);

	glEnable(GL_TEXTURE_2D);							//Enable texturing!

	// Also, do any other setting ov variables here for your app if you wish. 
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);// Linear Filtering
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);// Linear Filtering

}


bool NeHeWindow::SetGLPixelFormat() 
{ 
	///////////////////////////////////////////////////////////////////////////////
    // SetGLPixelFormat()
    // Sets up the OpenGL pixel format.
    ///////////////////////////////////////////////////////////////////////////////
    PIXELFORMATDESCRIPTOR pfd = {0}; 
    int pixelformat; 
 
    pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);	// Set the size of the structure
    pfd.nVersion = 1;							// Always set this to 1

	// Pass in the appropriate OpenGL flags
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER; 
    pfd.dwLayerMask = PFD_MAIN_PLANE;			// standard mask (this is ignored anyway)
    pfd.iPixelType = PFD_TYPE_RGBA;				// RGB and Alpha pixel type
    pfd.cColorBits = COLOUR_DEPTH;				// Here we use our #define for the color bits
    pfd.cDepthBits = COLOUR_DEPTH;				// Ignored for RBA
    pfd.cAccumBits = 0;							// nothing for accumulation
    pfd.cStencilBits = 0;						// nothing for stencil
 

	// Change these to asserts
	// Gets a best match on the pixel format as passed in from device

    if ( (pixelformat = ChoosePixelFormat(ghdc, &pfd)) == false ) 
    { 
        MessageBox(NULL, "ChoosePixelFormat failed", "Error", MB_OK); 
        return false; 
    } 
 
	// sets the pixel format if its ok. 
    if (SetPixelFormat(ghdc, pixelformat, &pfd) == false) 
    { 
        MessageBox(NULL, "SetPixelFormat failed", "Error", MB_OK); 
        return false; 
    } 
 
    return true;
}





bool NeHeWindow::Initialise()
{	
	///////////////////////////////////////////////////////////////////////////////
    // Initialise()
    // Constructs the window and OpenGL renderer but does NOT show / activate it. 
    ///////////////////////////////////////////////////////////////////////////////

	run_client_update_loop_ = false;



	WNDCLASS wcex;

	memset(&wcex, 0, sizeof(WNDCLASS));
	wcex.style			= CS_HREDRAW | CS_VREDRAW;		
	wcex.lpfnWndProc	= WndProcShared;			// the STATIC callback function.
	wcex.hInstance		= g_app_instance;						
	wcex.hIcon			= LoadIcon(NULL, IDI_APPLICATION);; 
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);		
	wcex.hbrBackground	= (HBRUSH) (COLOR_WINDOW+1);
	wcex.lpszMenuName	= NULL;	
	wcex.lpszClassName	= "WindowClass";	
	wcex.cbWndExtra     = sizeof(NeHeWindow*);

	RegisterClass(&wcex);// Register the class

	DWORD	dwStyle = WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
	DWORD  dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;

	// Set the Client area of the window to be our resolution.
	RECT glwindow;
	glwindow.left		= 0;		
	glwindow.right		= S_WIDTH;	
	glwindow.top		= 0;		
	glwindow.bottom		= S_HEIGHT;	

	AdjustWindowRect( &glwindow, dwStyle, false);

	// Create the window
	hwnd = CreateWindowEx(	dwExStyle,
							"WindowClass", 
							"FrameWorkWindow", 
							dwStyle, 
							0, 
							0,
							glwindow.right  - glwindow.left,
							glwindow.bottom - glwindow.top, 
							NULL,
							NULL,
							g_app_instance,
							this
							);

	assert(hwnd);

	// Call initialise here.
	LONG_PTR p = ::GetWindowLongPtr(hwnd,0);
	NeHeWindow * pThis = (NeHeWindow*) p;
	assert( pThis == this );

	// Initialise openGL rendering area in the window. 
	OpenGLInit();

	return IWindow::Initialise();
}

void NeHeWindow::ShutDownWindow()
{
	///////////////////////////////////////////////////////////////////////////////
    // ShutDownWindow()
    // Cleans up our window, Removes handles to devies etc. 
    ///////////////////////////////////////////////////////////////////////////////

	if (ghrc)
	{
		wglMakeCurrent(NULL, NULL);	// free rendering memory
		wglDeleteContext(ghrc);		// Delete our OpenGL Rendering Context
	}

	if (ghdc) 
	ReleaseDC(hwnd, ghdc);			// Release our HDC from memory

	UnregisterClass("WindowClass", g_app_instance);// Free the window class

	PostQuitMessage (0);			// Post a QUIT message to the window
	IWindow::ShutDownWindow();
}



void NeHeWindow::WindowResize( const int newWidth, const int newHeight )
{	
	///////////////////////////////////////////////////////////////////////////////
    // WindowResize()
    // If the user has resized the window 
    ///////////////////////////////////////////////////////////////////////////////

	ResizeGLWindow(newWidth, newHeight);

	// Then tell game the window has changed.
	IWindow::WindowResize( newWidth, newHeight );
}

void  NeHeWindow::ResizeGLWindow(int width, int height)
{	
	///////////////////////////////////////////////////////////////////////////////
    // ResizeGLWindow()
    // adjusts the OpenGL renderer to match new window size. 
    ///////////////////////////////////////////////////////////////////////////////

	if (height==0)// Prevent A Divide By Zero error
	{
		height=1;// Make the Height Equal One
	}

	glViewport(0,0,width,height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	//calculate aspect ratio
	gluPerspective(45.0f,(GLfloat)width/(GLfloat)height, 1 ,10000.0f);

	glMatrixMode(GL_MODELVIEW);// Select The Modelview Matrix
	glLoadIdentity();// Reset The Modelview Matrix
}

const int NeHeWindow::WindowHeight() const
{
	return -1;
}

const int NeHeWindow::WindowWidth() const
{
	return -1;
}

NeHeWindow::~NeHeWindow()
{
	::SetWindowLong( hwnd, 0, 0 );
}

