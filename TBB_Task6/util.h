#ifndef UTIL_H
#define UTIL_H

#include <iostream>
#include <fstream>
#include <cstdlib>
#include "numeric.h"

#define GETBIT(x,pos)   ( ((x) & ( 0x1 << (pos) )) !=0 )

#define TBBGetBit(x, pos)	\
  ((pos) <= 31) ? GETBIT(*(((unsigned int*)&(x)) + 1),(31 - (pos))) : GETBIT(*(((unsigned int*)&(x))),(63 - (pos) + 32))

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
	void TBBFree(uint len, double *mem);
};

void TBBOutput(bool out, const char* file, const char* text, double* array, uint len, uint precision);

void TBBParseArgs(int argc, char** argv, double &min, double &max, uint &num, uint &precision);

void TBBInitMemoryPool(uint len);

void TBBTerminateMemoryPool(void);

TBBPool *TBBGetMemoryPool(void);

int getCountOfThreads(void);

void setCountOfThreads(int num);

#endif