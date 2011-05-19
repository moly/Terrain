#ifndef _FAULT_FORMATION_H
#define _FAULT_FORMATION_H

#include <vector>
#include "vmath.h"

class FaultFormation
{
public:

	FaultFormation(int width, int depth);

	void GenerateHeightMap(std::vector<Vector3f>& vertices);

	void ApplyErosionFilter(std::vector<Vector3f>& vertices, float filterLevel);

private:

public:

private:

	int width;
	int depth;

	int iterations;
};

#endif