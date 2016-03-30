#pragma once

#include "sort.h"
#include "tbb/task_scheduler_init.h"
#include "tbb/parallel_for.h"
#include "tbb/blocked_range.h"
#include "tbb/mutex.h"
#include <iostream>
#include <stack>
//
using namespace std;
using namespace tbb;
#include <cmath>
#define uint unsigned long long int
#define NUM_VAL 10

class TBBCountOfDigits
{
	const double* numbers;
	uint len;
	uint & Max;
public:
	TBBCountOfDigits(const double* tnumbers, uint tlen, uint &tmax) :
		numbers(tnumbers), len(tlen), Max(tmax) {}

	uint TBBGetCountOfDigits(const double tnumber) const
	{
		int count = (tnumber == 0.0) ? 1 : 0;
		double fraction_, integer_;
		int integerInt = 0;
		fraction_ = modf(tnumber, &integer_);
		//cout << "fract = " << fraction << " " << "integer = " << integer << endl;
		integerInt = (int)integer_;
		count = (int)TBBGetCountOfDigitsInteger(integerInt);

		return count;
	}

	uint TBBGetCountOfDigitsInteger(int tinteger) const
	{
		int count = (tinteger == 0) ? 1 : 0;

		while (tinteger != 0)
		{
			count++;
			tinteger /= 10;
		}

		return count;
	}

	void operator() (const blocked_range<int>& r) const
	{
		uint max_ = 0;
		uint begin = r.begin(), end = r.end();
		for (uint i = begin; i < end; i++)
		{
			uint current = TBBGetCountOfDigits(numbers[i]);
			if (current > max_)
			{
				max_ = current;
			}
		}
		Max = max_;
	}
};

class TBBStackDistribution
{
	stack<double>* stackNeg;
	stack<double>* stackPos;
	uint len;
	const double* array;
	uint radix;
	uint count;
	static mutex Mutex;
public:
	TBBStackDistribution(const double* tarray, uint tlen, stack<double> *stPos, 
						 stack<double> *stNeg, uint tradix, uint tcount) :
		array(tarray), len(tlen), radix(tradix), count(tcount) 
	{
		stackPos = stPos;
		stackNeg = stNeg;
	}
	void operator() (const blocked_range<int>& r) const
	{
		uint begin = r.begin(), end = r.end();
		for (int i = r.begin(); i < r.end(); ++i)
		{
			double val = array[i];
			if (val >= 0.0)
			{
				mutex::scoped_lock lock;
				lock.acquire(Mutex);
				stackPos[(int)(val / pow(10, count - radix))].push(val);
				lock.release();
			}
			else
			{
				mutex::scoped_lock lock;
				lock.acquire(Mutex);
				val = val * (-1);
				stackNeg[(int)(val / pow(10, count - radix))].push(val);
				lock.release();
			}
		}
	}
};

class TBBSortMainLoop
{
	stack<double>* stackNeg;
	stack<double>* stackPos;
	uint len;
	double* result;
	uint radix;
	uint count;
	uint & counter;
	uint precision;
	bool switchPosToNeg;
	static mutex Mutex;
public:
	TBBSortMainLoop(double* tresult, uint tlen, stack<double> *stPos,
		stack<double> *stNeg, uint tradix, uint tcount, uint & tcounter, uint tprecision, bool tswitchPosToNeg) :
		result(tresult), len(tlen), radix(tradix), count(tcount), counter(tcounter), precision(tprecision), switchPosToNeg(tswitchPosToNeg)
	{
		stackPos = stPos;
		stackNeg = stNeg;
	}
	void operator() (const blocked_range<int>& r) const
	{
		uint begin = r.begin(), end = r.end();
		int j = 0;
		if (!switchPosToNeg)
		{
			for (int i = r.end(); i >= r.begin(); --i)
			{
				double* res = TBBRadixSortMSDStack(count, stackPos[i], precision, radix + 1);
				if (NULL != res)
				{
					for (j = 0; j < (int)stackPos[i].size(); j++)
					{
						result[counter] = res[j];
						counter++;
					}
				}
			}
		}
		else
		{
			for (int i = r.begin(); i < r.end(); ++i)
			{
				double* res = TBBRadixSortMSDStack(count, stackNeg[i], precision, radix + 1);
				if (NULL != res)
				{
					for (j = (int)stackNeg[i].size() - 1; j >= 1; --j)
					{
						result[counter] = res[j] * (-1);
						counter++;
					}
					result[counter] = res[0] * (-1);
					counter++;
				}
			}
		}
	}
};