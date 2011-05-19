#ifndef _DIRECTINPUT_H
#define _DIRECTINPUT_H

#include <d3d9.h>
#include <d3dx9.h>
#include <dinput.h>   
#include "iinput.h"

class DirectInput : public IInput 
{
public:
	DirectInput();

	virtual ~DirectInput();

	virtual void Update();

private:

	virtual bool Initialise();

	virtual	bool GetDevices();

	virtual void ShutDown();

public: 

	

private:

	LPDIRECTINPUT8	DIObject;				//DirectInput main object
	LPDIRECTINPUTDEVICE8 DIKeyboardDevice;	//keyboard device.
	char			KeyBuffer[256];			//Buffer that holds the key input state
	

};


#endif

