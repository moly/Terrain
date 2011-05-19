#include "terrain.h"

Terrain::Terrain(int width, int depth)
{
	this->width = width; this->depth = depth;
	heightScale = 0.2f;

	int numVertices = depth * width;
	vertices.reserve(numVertices);

	// fill vector with points
	for(int i = 0; i < numVertices; ++i)
	{
		vertices.push_back(Vector3f((float)(i % width), 0.0f, (float)(int)(i / width)));
	}

	// make our terrain using fault formation
	faultFormation = new FaultFormation(width, depth);
	faultFormation->GenerateHeightMap(vertices);
	faultFormation->ApplyErosionFilter(vertices, 0.4f);
	
	ApproximateNormals();
	
	GenerateTexture();	
}

void Terrain::Render()
{
	for(int z = 0; z < depth - 1; ++z)
	{	
		glBegin(GL_TRIANGLE_STRIP);
		for(int x = 0; x < width; ++x)
		{
			glNormal3f(VertexAt(x, z).nx, VertexAt(x, z).ny, VertexAt(x, z).nz);
			glMultiTexCoord2fARB(GL_TEXTURE0_ARB, (float)x / width, (float)z / depth);
			//glMultiTexCoord2fARB(GL_TEXTURE1_ARB, 0.0f, x%2);
			glVertex3f(x -(width * 0.5) , VertexAt(x, z).y, z -(depth * 0.5));

			glNormal3f(VertexAt(x, z + 1).nx, VertexAt(x, z + 1).ny, VertexAt(x, z + 1).nz);
			glMultiTexCoord2fARB(GL_TEXTURE0_ARB, (float)x / width, (float)(z + 1) / depth);
			//glMultiTexCoord2fARB(GL_TEXTURE1_ARB, 1.0f, x%2);
			glVertex3f(x -(width * 0.5), VertexAt(x, z + 1).y, z + 1 -(depth * 0.5));
		}
		glEnd();
	}
}

void Terrain::Regenerate()
{
	faultFormation->GenerateHeightMap(vertices);
	faultFormation->ApplyErosionFilter(vertices, 0.4f);
	GenerateTexture();
	ApproximateNormals();
}

void Terrain::RandomizeHeights()
{
	for(unsigned int i = 0; i < vertices.size(); ++i)
		vertices[i].y = (float)(rand() % 255);

	ApproximateNormals();
}

void Terrain::GenerateHeightMapFaultFormation()
{
	
}

inline // allows access to the vector as if it were two dimensional
Vector3f Terrain::VertexAt(const int row, const int column)
{
	return (row >= width || column >= depth || row < 0 || column < 0) ? Vector3f() : TrueVertexAt(row, column) * Vector3f(1, heightScale, 1);
}

inline // allows access to the vector as if it were two dimensional
Vector3f Terrain::TrueVertexAt(const int row, const int column)
{
	return vertices[row + (column * width)];
}

void Terrain::ApproximateNormals()
{
	for(int i = 1; i < width - 2; i++)
	{
		for(int j = 1; j < depth - 2; j++)
		{
			Vector3f v = (VertexAt(i + 1, j) - VertexAt(i - 1, j)).crossProduct(VertexAt(i, j + 1) - VertexAt(i, j - 1));
			Vector3f& cv = VertexAt(i, j);
			cv.nx = v.x; cv.ny = v.y; cv.nz = v.z;
		}
	}
}

void Terrain::GenerateTexture()
{
	TEXTURE textures[2];
	int numTextures = 2;

	// Load in textures
	LoadTexture("grass.tga", textures[0]);
	LoadTexture("snow.tga", textures[1]);

	// Give each texture a height range
	int lastHeight = -1;
	for(int i = 0; i < numTextures; ++i)
	{
		textures[i].lowHeight = lastHeight + 1;
		lastHeight += 255 / numTextures;
		textures[i].optimalHeight = lastHeight;
		textures[i].highHeight = (lastHeight - textures[i].lowHeight) + lastHeight;
	}

	float scale = (float)width / textures[0].width;

	// Cycle through all the textures, take a pixel from each one
	// and mix them together based on the height of the terrain at
	// that point
	GLubyte texture_imageData[256][256][3];
	for(int y = 0; y < textures[0].height; ++y)
	{
		for(int x = 0; x < textures[0].width; ++x)
		{
			float red = 0.0f, green = 0.0f, blue = 0.0f;
			for(int i = 0; i < numTextures; ++i)
			{
				red += (textures[i].RedAt(x, y) * textures[i].PixelPercentageAtHeight(TrueVertexAt((int)(x * scale), (int)(y * scale)).y));
				green += (textures[i].GreenAt(x, y) * textures[i].PixelPercentageAtHeight(TrueVertexAt((int)(x * scale), (int)(y * scale)).y));
				blue += (textures[i].BlueAt(x, y) * textures[i].PixelPercentageAtHeight(TrueVertexAt((int)(x * scale), (int)(y * scale)).y));
			}
			texture_imageData[y][x][0] = red;
			texture_imageData[y][x][1] = green;
			texture_imageData[y][x][2] = blue;
		}
	}
	
	// Typical Texture Generation Using Data From The TGA ( CHANGE )
	glGenTextures(1, &texture1);				// Create The Texture ( CHANGE )
	glBindTexture(GL_TEXTURE_2D, texture1);
	glTexImage2D(GL_TEXTURE_2D, 0, 24 / 8, textures[0].width, textures[0].height, 0, GL_RGB, GL_UNSIGNED_BYTE, texture_imageData);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

	glActiveTextureARB(GL_TEXTURE0_ARB);		//Set up texture Unit1
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,  texture1);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	
	glActiveTextureARB(GL_TEXTURE1_ARB);		//Set up texture unit 2
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,  texture2);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
}