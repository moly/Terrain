#ifndef _IGAME_H
#define _IGAME_H

class IGame 
{
public:
	virtual	bool Initialise()	=	0;

	virtual void Update(const float dtSeconds) = 0;

	virtual void ShutDown()		=	0;

	virtual void OnWindowResize( const int newWidth, const int newHeight ) = 0;
};


#endif // IGAME_H

