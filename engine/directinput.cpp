#include "directinput.h"
#include "CAssert"

void DirectInput::Update()
{	
	///////////////////////////////////////////////////////////////////////////////
    // Update Function
    // Reads the active input devices (keyboard), maps into the Base class.
    ///////////////////////////////////////////////////////////////////////////////

	//obtain the current keyboard state and pack it into the keybuffer 
	bool DeviceState = SUCCEEDED(DIKeyboardDevice->GetDeviceState(sizeof(KeyBuffer),
								(LPVOID)&KeyBuffer));
	assert(DeviceState);

	//Map key inputs to Control maps here
	//the control maps are part of the base class and
	//we check them in the actual application and take action on them. 
	//if(KeyBuffer[DIK_F10] & 0x80)
	//{
	//	DigitalControlMap[0]=true;
	//}
	for(int i = 0; i < DIGITALCONTROLMAPS; i++)
		if(KeyBuffer[i] & 0x80)
			DigitalControlMap[i] = true;
		else
			DigitalControlMap[i] = false;
}


DirectInput::DirectInput()
{
	///////////////////////////////////////////////////////////////////////////////
    // Constructor
    // Initialises Direct Input, sets up and aquires input devices. 
    ///////////////////////////////////////////////////////////////////////////////
	
	DIObject = NULL;				//set Directinput object pointer to null.
	DIKeyboardDevice = NULL;		//keyboard device set to null.
	
	bool InputInitialised =Initialise();
	assert(InputInitialised);

	bool DevicesInitialised = GetDevices();
	assert(DevicesInitialised);

	//Initialise the Analogue and digital control maps
	for (int i=0; i<DIGITALCONTROLMAPS; i++)
	{
		DigitalControlMap[i] = false;
	}
	for (int i=0; i<ANALOGUECONTROLMAPS; i++)
	{
		AnalogueControlMap[i];
	}

}

	

DirectInput::~DirectInput()
{	
	///////////////////////////////////////////////////////////////////////////////
    // Destructor
    // 
    ///////////////////////////////////////////////////////////////////////////////


	
}



bool DirectInput::Initialise()
{	
	///////////////////////////////////////////////////////////////////////////////
    // Initialise function
    // Create the Direct Input base object. 
    ///////////////////////////////////////////////////////////////////////////////

	
	if(FAILED(DirectInput8Create(	GetModuleHandle(NULL),
									DIRECTINPUT_VERSION,
									IID_IDirectInput8,
									(void**)&DIObject,
									NULL)))
	{

		assert (DIObject);
		return false;
	}
	return true;
}

bool DirectInput::GetDevices()
{	
	/////////////////////////////////////////////////////////////////////////////////////////
    // Get input devices
    // This is seperate from "initialise" in case it must be called when unplugging things etc.
    /////////////////////////////////////////////////////////////////////////////////////////

	// possibly change to: bool = failed(function) and assert
	// the bool when failed.  Will always assert when fails
	// but may help pick out the error point from the assert
	// message. 

	// Create the Device
	if(FAILED(DIObject->CreateDevice(GUID_SysKeyboard,
                                    &DIKeyboardDevice,
                                    NULL)))
	{
		return false;
	}
	
	// Set the Data format (c_dfDIKeyboard is standard Dirx Global)
	if(FAILED(DIKeyboardDevice->SetDataFormat(&c_dfDIKeyboard)))
	{
		return false;
	}
	
	// How the app handles control of keyboard when switching window etc. 
	if(FAILED(DIKeyboardDevice->SetCooperativeLevel(NULL,
                                                   DISCL_BACKGROUND | 
                                                   DISCL_NONEXCLUSIVE)))
	{	
		return false;
	}
	
	// Aquiring the keyboard now everything is set up.
	if(FAILED(DIKeyboardDevice->Acquire()))

	{	
		return false;
	}

	return true;

}

void DirectInput::ShutDown()
{
	///////////////////////////////////////////////////////////////////////////////
    // Shutdown
    // Anything else that we need to clean up,  do it here. 
    ///////////////////////////////////////////////////////////////////////////////

	if(DIObject != NULL)
		{
			DIObject->Release(); 
			DIObject = NULL;
		}

		if(DIKeyboardDevice != NULL)
		{
			DIKeyboardDevice->Unacquire();	
			 DIKeyboardDevice->Release();   
			DIKeyboardDevice = NULL;		
		}
}
