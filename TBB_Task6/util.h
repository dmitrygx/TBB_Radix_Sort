#ifndef UTIL_H
#define UTIL_H

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <omp.h>
#include "numeric.h"

using namespace std;

#define uint unsigned long long int

class TBBPool
{
private:
	double* pool;
	uint size;
	double* current;
	uint currentSize;
public:
	TBBPool(uint sizeOfMem = 0)
	{
		size = sizeOfMem;
		currentSize = 0;
		TBBMemoryPoolAlloc(size);
	}
	~TBBPool()
	{
		size = 0;
		currentSize = 0;
	}
	void TBBMemoryPoolAlloc(uint len);
	void TBBMemoryPollFree();
	double* TBBAlloc(uint len);
};

void TBBOutput(bool out, const char* file, const char* text, double* array, uint len);

void TBBParseArgs(int argc, char** argv, double &min, double &max, uint &num, uint &precision);

void TBBInitMemoryPool(uint len);

void TBBTerminateMemoryPool(void);

TBBPool *TBBGetMemoryPool(void);

#endif
