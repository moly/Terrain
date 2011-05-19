#include "light.h"

Light::Light(float ar, float ag, float ab, float dr, float dg, float db, float x, float y, float z)
{
	lightAmbient[0] = ar;
	lightAmbient[1] = ag;
	lightAmbient[2] = ab;
	lightAmbient[3] = 1.0f;

	lightDiffuse[0] = dr;
	lightDiffuse[1] = dg;
	lightDiffuse[2] = db;
	lightDiffuse[3] = 1.0f;

	lightPosition[0] = x;
	lightPosition[1] = y;
	lightPosition[2] = z;
	lightPosition[3] = 1.0f;

	glLightfv(GL_LIGHT1, GL_AMBIENT, lightAmbient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, lightDiffuse);
	glLightfv(GL_LIGHT1, GL_POSITION, lightPosition);
}

void Light::Enable()
{
	glEnable(GL_LIGHT1);
}