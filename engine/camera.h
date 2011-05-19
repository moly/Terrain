#ifndef _CAMERA_H
#define _CAMERA_H

#include <GL/glut.h>

class Camera
{
public:

	void Update(float strafe, float advance, float upDown, float xRot, float yRot, int terrainHeight);

	void PositionCamera(float posx, float posy, float posz, float xRot, float yRot);

	int GetX(){return x;};

	int GetZ(){return z;};

private:

public:

	float movementSpeed;
	bool collisionOn;

private:

	float x;
	float y;
	float z;
	float xRotation;
	float yRotation;

};

#endif