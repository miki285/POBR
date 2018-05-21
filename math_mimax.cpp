#include "math_mimax.h"
#include <algorithm>

unsigned int maximum(int R, int G, int B)
{
	int temp = std::max(R, G);
	return std::max(temp, B);
}

unsigned int minimum(int R, int G, int B)
{
	int temp = std::min(R, G);
	return std::min(temp, B);
}