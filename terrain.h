#ifndef _TERRAIN_H
#define _TERRAIN_H

#include <vector>
#include <GL/glew.h>
#include "vmath.h"
#include "tex.h"
#include "faultFormation.h"

class Terrain
{
public:

	Terrain(int width, int depth);

	void Update(const float dtSeconds);

	void Render();

	void Regenerate();

	void RandomizeHeights();

	void GenerateHeightMapFaultFormation();

	Vector3f VertexAt(const int row, const int column);

private:

	Vector3f TrueVertexAt(const int row, const int column);

	void ApproximateNormals();

	void GenerateTexture();

public:

private:

	int width;
	int depth;
	float heightScale;

	unsigned int texture1;
	unsigned int texture2;

	std::vector<Vector3f> vertices;

	FaultFormation* faultFormation;

};

#endif