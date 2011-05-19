#include "camera.h"

void Camera::Update(float strafe, float advance, float upDown, float xRot, float yRot, int terrainHeight)
{
	if(collisionOn == true)
	{
		glRotatef(xRotation += xRot, 1.0f, 0.0f, 0.0f);
		glRotatef(yRotation += yRot, 0.0f, 1.0f, 0.0f);
		y = terrainHeight - 10;
		glTranslatef(x += strafe, y += upDown, -(z += advance));
		//if(y < terrainHeight)
			//y = terrainHeight;
	}
	else
	{
		glTranslatef(x += strafe, y += upDown, -(z += advance));
		glRotatef(xRotation += xRot, 1.0f, 0.0f, 0.0f);
		glRotatef(yRotation += yRot, 0.0f, 1.0f, 0.0f);
	}
}

void Camera::PositionCamera(float posx, float posy, float posz, float xRot, float yRot)
{
	x = posx;
	y = posy;
	z = posz;
	xRotation = xRot;
	yRotation = yRot;
}