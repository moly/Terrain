#ifndef _TEX_H_
#define _TEX_H_

#include <windows.h>
#include <stdio.h>
#include <gl/gl.h>
#include <gl/glu.h>


//#define uTGAcompare  {0,0,2, 0,0,0,0,0,0,0,0,0};	// Uncompressed TGA Header
//#define cTGAcompare  {0,0,10,0,0,0,0,0,0,0,0,0};	// Compressed TGA Header

typedef struct
{
	GLubyte Header[12];									// TGA File Header
} TGAHeader;

typedef struct
{
	GLubyte	*imageData;
	GLuint width; 
	GLuint height;

	GLuint lowHeight;
	GLuint optimalHeight;
	GLuint highHeight;

	GLubyte RedAt(int x, int y)
	{
		return imageData[(y * width * 3) + (x * 3)];
	}
	GLubyte GreenAt(int x, int y)
	{
		return imageData[(y * width * 3) + (x * 3) + 1];
	}
	GLubyte BlueAt(int x, int y)
	{
		return imageData[(y * width * 3) + (x * 3) + 2];
	}

	GLfloat PixelPercentageAtHeight(GLuint terrainHeight)
	{
		if(terrainHeight == optimalHeight)
			return 1.0f;

		else if(terrainHeight < lowHeight || terrainHeight > highHeight)
			return 0.0f;

		else if(terrainHeight < optimalHeight)
			return (float)(terrainHeight - lowHeight) / (float)(optimalHeight - lowHeight);
			
		else if(terrainHeight > optimalHeight)
			return (((float)(highHeight - optimalHeight) - (float)(terrainHeight - optimalHeight)) / (float)(highHeight - optimalHeight));

		return 0.0f;
	}

} TEXTURE;

typedef struct
{
	GLubyte		header[6];								// First 6 Useful Bytes From The Header
	GLuint		bytesPerPixel;							// Holds Number Of Bytes Per Pixel Used In The TGA File
	GLuint		imageSize;								// Used To Store The Image Size When Setting Aside Ram
	GLuint		temp;									// Temporary Variable
	GLuint		type;	
	GLuint		Height;									//Height of Image
	GLuint		Width;									//Width ofImage
	GLuint		Bpp;									// Bits Per Pixel
} TGA;


bool LoadUncompressedTGA(char * filename, FILE * fTGA);
bool LoadCompressedTGA( char * filename, FILE * fTGA);
bool CreateGLTexture(char *name, GLuint & TexID  );
bool LoadTexture(char * filename, TEXTURE& texture);

#endif 