#include "faultFormation.h"

FaultFormation::FaultFormation(int width, int depth)
{
	this->width = width;
	this->depth = depth;
}

void FaultFormation::GenerateHeightMap(std::vector<Vector3f>& vertices)
{
	iterations = 64;
	for(int i = 0; i < iterations; ++i)
	{
		// Choose two different sides of the heightmap
		int wall1 = rand() % 4;
	    int wall2;

	    do
	    {
	        wall2 = rand() % 4;
	    }
		while (wall1 == wall2);

		Vector2f v1;
	    Vector2f v2;

		// Choose random points on each side to form a
		// fault line
	    while (v1.x == v2.x || v1.y == v2.y)
	    {
	    	switch(wall1)
			{
				case 0:
	        		v1.x = rand() % width;
	            	v1.y = 0;
	        		break;

				case 1:
	        		v1.y = rand() % depth;
	            	v1.x = width;
					break;
	  
				case 2:
	        		v1.x = rand() % width;
	            	v1.y = depth;
					break;
	        
				case 3:
	            	v1.x = 0;
	            	v1.y = rand() % depth;
	        		break;
			}

			switch(wall2)
			{
				case 0:
	        		v2.x = rand() % width;
	            	v2.y = 0;
					break;

				case 1:
	        		v2.y = rand() % depth;
	            	v2.x = width;
					break;

				case 2:
	        		v2.x = rand() % width;
	            	v2.y = depth;
					break;

				case 3:
	        		v2.x = 0;
	            	v2.y = rand() % depth;
					break;
	        }
	    }

		// create a vector going the same direction as the line
		Vector2f v3 = v2 - v1;

		for(int x = 0; x < width; ++x)
		{
			for(int y = 0; y < depth; ++y)
			{
				// create a vector from the start of the line to the current point in the loop
				Vector2f v4 = Vector2f(x - v1.x, y - v1.y);

				// if the z component of the cross product is greater than zero raise the point
				if((v4.x * v3.y - v3.x * v4.y) > 0)
					vertices[x + (y * width)].y += (255 - (255 * i) / iterations);
			}
		}
	}
	
	// find the lowest and highest points
	int lowest = vertices[0].y, highest = 0;
	for(int i = 0; i < width * depth; ++i)
	{
		if(vertices[i].y < lowest)
			lowest = vertices[i].y;

		if(vertices[i].y > highest)
			highest = vertices[i].y;
	}

	// move everything down to the centre of the screen
	// and put into range 0 - 255
	float scale = 255.f / (highest - lowest);
	for(int i = 0; i < width * depth; ++i)
	{
		vertices[i].y -= lowest;
		vertices[i].y *= scale;
	}
}

// simulate terrain erosion
void FaultFormation::ApplyErosionFilter(std::vector<Vector3f>& vertices, float filterLevel)
{
	float acc;
    float acckx, accky;
	std::vector<Vector3f> temp = vertices;

	// first pass X direction
    accky = 1.0f / (1.0f - filterLevel);
    for (int y = 0; y < depth; ++y)
	{
        acc = temp[y * width].y * accky;
        for (int x = 0; x < width; ++x)
		{
            vertices[x + (y * width)].y = acc / accky;
            acc = acc * filterLevel + temp[x + (y * width)].y;
        }
    }
	
    // second pass X direction
    for (int y = 0; y < depth; ++y)
	{
    	acc = temp[(width - 1) + (y * width)].y * accky;
        for (int x = width - 1; x >= 0; --x)
		{
        	vertices[x + (y * width)].y += acc / accky;
            acc = acc * filterLevel + temp[x + (y * width)].y;
        }
    }
    
    // first pass Y direction
    acckx = 1.0f / (1.0f - filterLevel);
    for (int x = 0; x < width; ++x)
	{
        acc = temp[x].y * acckx;
        for (int y = 0; y < depth; ++y)
		{
            vertices[x + (y * width)].y += acc / acckx;
            acc = acc * filterLevel + temp[x + (y * width)].y;
        }
    }

    // second pass Y direction
    for (int x = 0; x < width; ++x)
	{
    	acc = temp[x + ((width - 1) * width)].y * acckx;
        for (int y = depth - 1; y >= 0; --y)
		{
        	vertices[x + (y * width)].y += acc / acckx;
            acc = acc * filterLevel + temp[x + (y * width)].y;
        }
    }
    
    // averaging for 4 passes 
    for (int x = 0; x < width; ++x)
	{
        for (int y = 0; y < depth; ++y)
		{
            vertices[x + (y * width)].y /= 4;
        }
    }
	
}