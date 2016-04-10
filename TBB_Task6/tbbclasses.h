#pragma once

#include "sort.h"
#include "tbb/task_scheduler_init.h"
#include "tbb/parallel_for.h"
#include "tbb/blocked_range.h"
#include "tbb/mutex.h"
#include <iostream>
#include <stack>
#include "sort.h"
//
using namespace std;
using namespace tbb;
#include <cmath>
#define uint unsigned long long int
#define NUM_VAL 2

//class TBBCountOfDigits
//{
//	const double* numbers;
//	uint len;
//	uint & Max;
//public:
//	TBBCountOfDigits(const double* tnumbers, uint tlen, uint &tmax) :
//		numbers(tnumbers), len(tlen), Max(tmax) {}
//
//	uint TBBGetCountOfDigits(const double tnumber) const
//	{
//		int count = (tnumber == 0.0) ? 1 : 0;
//		double fraction_, integer_;
//		int integerInt = 0;
//		fraction_ = modf(tnumber, &integer_);
//		//cout << "fract = " << fraction << " " << "integer = " << integer << endl;
//		integerInt = (int)integer_;
//		count = (int)TBBGetCountOfDigitsInteger(integerInt);
//
//		return count;
//	}
//
//	uint TBBGetCountOfDigitsInteger(int tinteger) const
//	{
//		int count = (tinteger == 0) ? 1 : 0;
//
//		while (tinteger != 0)
//		{
//			count++;
//			tinteger /= 10;
//		}
//
//		return count;
//	}
//
//	void operator() (const blocked_range<int>& r) const
//	{
//		uint max_ = 0;
//		uint begin = r.begin(), end = r.end();
//		for (uint i = begin; i < end; i++)
//		{
//			uint current = TBBGetCountOfDigits(numbers[i]);
//			if (current > max_)
//			{
//				max_ = current;
//			}
//		}
//		Max = max_;
//	}
//};
//
//class TBBStackDistribution
//{
//	stack<double>* stackNeg;
//	stack<double>* stackPos;
//	uint len;
//	const double* array;
//	uint radix;
//	uint count;
//	static mutex Mutex;
//public:
//	TBBStackDistribution(const double* tarray, uint tlen, stack<double> *stPos, 
//						 stack<double> *stNeg, uint tradix, uint tcount) :
//		array(tarray), len(tlen), radix(tradix), count(tcount) 
//	{
//		stackPos = stPos;
//		stackNeg = stNeg;
//	}
//	void operator() (const blocked_range<int>& r) const
//	{
//		uint begin = r.begin(), end = r.end();
//		for (int i = r.begin(); i < r.end(); ++i)
//		{
//			double val = array[i];
//			if (val >= 0.0)
//			{
//				mutex::scoped_lock lock;
//				lock.acquire(Mutex);
//				stackPos[(int)(val / pow(10, count - radix))].push(val);
//				lock.release();
//			}
//			else
//			{
//				mutex::scoped_lock lock;
//				lock.acquire(Mutex);
//				val = val * (-1);
//				stackNeg[(int)(val / pow(10, count - radix))].push(val);
//				lock.release();
//			}
//		}
//	}
//};
//
//double* TBBRadixSortMSDStack(uint count, stack<double> st, uint precision, uint radix);
//double* TBBRadixSortMSDStackDouble(uint count, stack<double> st, uint precision, uint radix);
//
//class TBBSortMainLoop
//{
//	stack<double>* stackNeg;
//	stack<double>* stackPos;
//	uint len;
//	double** res;
//	uint radix;
//	uint count;
//	uint & counter;
//	uint precision;
//	bool switchPosToNeg;
//	static mutex Mutex;
//public:
//	TBBSortMainLoop(double** tresult, uint tlen, stack<double> *stPos,
//		stack<double> *stNeg, uint tradix, uint tcount, uint & tcounter, uint tprecision, bool tswitchPosToNeg) :
//		res(tresult), len(tlen), radix(tradix), count(tcount), counter(tcounter), precision(tprecision), switchPosToNeg(tswitchPosToNeg)
//	{
//		stackPos = stPos;
//		stackNeg = stNeg;
//	}
//	void operator() (const blocked_range<int>& r) const
//	{
//		uint begin = r.begin(), end = r.end();
//		if (!switchPosToNeg)
//		{
//			for (int i = r.end(); i >= r.begin(); --i)
//			{
//				res[i] = TBBRadixSortMSDStack(count, stackPos[i], precision, radix + 1);
//				/*if (NULL != res[i])
//				{
//					for (int j = 0; j < (int)stackPos[i].size(); j++)
//					{
//						cout << i << " = " << res[i][j] << endl;
//					}
//				}*/
//			}
//		}
//		else
//		{
//			for (int i = r.begin(); i < r.end(); ++i)
//			{
//				res[i] = TBBRadixSortMSDStack(count, stackNeg[i], precision, radix + 1);
//			}
//		}
//	}
//};

class TBBMSDRadixSort1 : public task
{
	uint len{ 0 };
	uint full{ 0 };
	uint radix{ 0 };
	double *array;
	task* execute()
	{
		double *aux;
		uint thr1 = 0;
		uint thr0 = 0;
		double val = 0;
		unsigned int bit = 0;
		uint left0 = 0;
		uint right1 = len;
		uint counter1 = 0;
		uint counter2 = 0;
		if (len == 0)
		{
			return NULL;
		}
		else
		{
			if ((64 == radix) || (len == 1))
			{
				uint counter = 0;
				for (int i = 0; i < (int)len; ++i)
				{
					result[counter] = array[i];
					//cout << "res = " << result[counter] << endl;
					counter++;
				}
				return NULL;
			}
		}
		aux = TBBGetMemoryPool()->TBBAlloc(len);
		for (int i = 0; i < (int)len; ++i)
		{
			val = array[i];
			bit = TBBGetBit(val, radix);
			TBBAddNewElemToAuxArr(val, aux, bit, &left0, &right1);
			(!bit) ? thr0++ : thr1++;
		}
		double *result0 = TBBGetMemoryPool()->TBBAlloc(thr0);
		double *result1 = TBBGetMemoryPool()->TBBAlloc(thr1);
		task_list tasks;
		TBBMSDRadixSort1& tbbSort1 = *new (allocate_child()) TBBMSDRadixSort1(aux + thr0, thr1, radix + 1, full, result1);
		tasks.push_back(tbbSort1);
		TBBMSDRadixSort1& tbbSort0 = *new (allocate_child()) TBBMSDRadixSort1(aux, thr0, radix + 1, full, result0);
		tasks.push_back(tbbSort0);
		set_ref_count(3);
		//cout << "My radix = " << radix << " refcount = " << ref_count() << endl;
		spawn_and_wait_for_all(tasks);
		//cout << "My radix = " << radix << " refcount = " << ref_count() << endl;

		if (NULL != result0)
		{
			if (0 == radix)
			{
				for (int j = 0; j < (int)thr0; j++)
				{
					result[thr1 + counter1] = result0[j];
					//cout << radix << ": res0[" << j << "] = " << result[thr1 + counter1] << endl;
					counter1++;
				}
			}
			else
			{
				for (int j = 0; j < (int)thr0; j++)
				{
					result[counter1] = result0[j];
					//cout << radix << ": res0[" << j << "] = " << result[counter1] << endl;
					counter1++;
				}
			}
			//TBBGetMemoryPool()->TBBFree(thr1, tbbSort0.result);
		}

		if (NULL != result1)
		{
			if (0 == radix)
			{
				for (int j = (int)thr1 - 1; j >= 0; --j)
				{
					result[counter2] = result1[j];
					//cout << radix << ": res1[" << j << "] = " << result[counter2] << endl;
					counter2++;
				}
			}
			else
			{
				for (int j = 0; j < (int)thr1; j++)
				{
					result[thr0 + counter2] = result1[j];
					//cout << radix << ": res1[" << j << "] = " << result[thr0 + counter2] << endl;
					counter2++;
				}
			}
			//TBBGetMemoryPool()->TBBFree(thr0, tbbSort1.result);
		}
		TBBGetMemoryPool()->TBBFree(len, aux);
		return NULL;
	}
public:
	double *result;
	TBBMSDRadixSort1(double *tarray, uint tlen, uint tradix, uint tfull, double *tresult) :
		array(tarray), len(tlen), radix(tradix), full(tfull), result(tresult)
	{}
};