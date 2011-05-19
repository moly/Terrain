#ifndef _LIGHT_H
#define _LIGHT_H

#include <GL/glew.h>

class Light
{
public:
	Light(float ar, float ag, float ab, float dr, float dg, float db, float x, float y, float z);
	void Enable();

private:

public:

private:

	float lightAmbient[4];
	float lightDiffuse[4];
	float lightPosition[4];

};

#endif