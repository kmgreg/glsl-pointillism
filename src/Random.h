/*
@author = Lauren Cole
header file of simple random functions. 
potential issue: seed could potentially be reset by something else
its not an issue that affects its uses in this particle system implementation so it is ignored
*/

#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
class Random
{
public:
	/*
	constructor for the rand class. seed is decided upon class instatitation, or can optionally be passed in
	*/
	Random (int seed=time(0))
	{
		srand(seed);
	}

	/*
	generate a random float between 0 and 1
	*/
	float getRandFloat()
	{
		return (float) rand() / (float) RAND_MAX;
	}

	/*
	generate a random float between 2 other floats
	*/
	float getFloatInRange(float first, float second)
	{
		float f = getRandFloat();
		return first + f * (second - first);
	}
};

