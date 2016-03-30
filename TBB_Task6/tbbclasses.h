#pragma once
#include "tbb/task_scheduler_init.h"
#include "tbb/parallel_for.h"
#include "tbb/blocked_range.h"
#include <iostream>
//
using namespace std;
using namespace tbb;
#include <cmath>
#define uint unsigned long long int

class TBBCountOfDigits
{
	const double* numbers;
	uint len;
	uint & max;
public:
	TBBCountOfDigits(const double* tnumbers, uint tlen, uint &tmax) :
		numbers(tnumbers), len(tlen), max(tmax) {}

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
		max = max_;
	}
};