#ifndef _IINPUT_H
#define _IINPUT_H

#define DIGITALCONTROLMAPS 256
#define ANALOGUECONTROLMAPS 64

class IInput 
{
public:
	IInput() {}

	virtual ~IInput()	{}

	virtual void Update()		=	0;

private:

	virtual bool Initialise()	=	0;

	virtual	bool GetDevices()	=	0;

	virtual void ShutDown()		=	0;

public: 

	bool fullscreen;
	
	//make sure to initalise these in the constructor of the inheriting class.
	bool	DigitalControlMap [DIGITALCONTROLMAPS];
	float	AnalogueControlMap[ANALOGUECONTROLMAPS];

private:
	

};



#endif