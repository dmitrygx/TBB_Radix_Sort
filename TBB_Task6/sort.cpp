#include "sort.h"

//static uint TBBGetCountOfDigitsInteger(int integer)
//{
//  int count = (integer == 0) ? 1 : 0;
//
//  while (integer != 0)
//  {
//	  count++;
//	  integer /= 10;
//  }
//
//  return count;
//}
//
//static uint TBBGetCountOfDigits(double number, int &integ, double &fract)
//{
//  int count = (number == 0.0) ? 1 : 0;
//  double fraction, integer;
//  int integerInt = 0;
//  fraction = modf(number, &integer);
//  //cout << "fract = " << fraction << " " << "integer = " << integer << endl;
//  integerInt = (int) integer;
//  
//  integ = (int) integer;
//  fract = fraction;
//
//  count = (int)TBBGetCountOfDigitsInteger(integerInt);
//  
//  return count;
//}

static uint TBBGetMaxCountOfDigits(const double* numbers, uint len, int* integ, double* fract)
{
	uint max = 0;

	//#pragma omp parallel for schedule(dynamic, CHUNK)
	 /* for (int i = 0; i < len; ++i)
	  {
		uint current = TBBGetCountOfDigits(numbers[i], integ[0], fract[0]);
		if (current > max)
		{
		  max = current;
		}
	  }*/
	int grainsize = 2;
	parallel_for(blocked_range<int>(0, (int)len, (size_t)grainsize), TBBCountOfDigits(numbers, len, max));
	return max;
}

double* TBBRadixSortMSDStackDouble(uint count, stack<double> st, uint precision, uint radix)
{
	uint len = st.size();
	double* result = TBBGetMemoryPool()->TBBAlloc(len);
	stack<double> stack[NUM_VAL];

	if (st.empty())
	{
		return NULL;
	}
	else
	{
		if (precision < radix)
		{
			uint counter = 0;

			while (!st.empty())
			{
				result[counter] = st.top();
				st.pop();
				counter++;
			}
			return result;
		}
	}

	while (!st.empty())
	{
		double value = st.top();
		st.pop();
		stack[(int)(value * pow(10, radix)) % 10].push(value);
	}
	uint counter = 0;
	uint j = 0;
	//#pragma omp parallel shared(counter, stack) private(j)
	{
		//#pragma omp for schedule(dynamic, CHUNK) nowait
		for (int i = NUM_VAL - 1; i >= 0; i--)
		{
			double* res = TBBRadixSortMSDStackDouble(count, stack[i], precision, radix + 1);
			if (NULL != res)
			{
				for (j = 0; j < stack[i].size(); j++)
				{
					result[counter] = res[j];
					counter++;
				}
			}
		}
	}
	return result;
}

double* TBBRadixSortMSDStack(uint count, stack<double> st, uint precision, uint radix)
{
	uint len = st.size();
	double* result = TBBGetMemoryPool()->TBBAlloc(len);
	stack<double> stack[NUM_VAL];

	if (st.empty())
	{
		return NULL;
	}
	else
	{
		if (radix - 1 == count)
		{
			/*uint counter = 0;
			while (!st.empty())
			{
		  result[counter] = st.top();
		  //cout << "res[" << counter << "] = " << result[counter] << endl;
		  cout << "radix = " << radix << " count = " << count << endl;
		  st.pop();
		  counter++;
		  }*/
			result = TBBRadixSortMSDStackDouble(count, st, precision, 1);
			return result;
		}
	}

	while (!st.empty())
	{
		double value = st.top();
		st.pop();
		stack[(int)(value / pow(10, count - radix)) - (int)(value / pow(10, count - (radix - 1))) * 10].push(value);
	}
	uint counter = 0;
	uint j = 0;
	//#pragma omp parallel shared(counter, stack) private(j)
	{
		//#pragma omp for schedule(dynamic, CHUNK) nowait
		for (int i = NUM_VAL - 1; i >= 0; i--)
		{
			double* res = TBBRadixSortMSDStack(count, stack[i], precision, radix + 1);
			if (NULL != res)
			{
				for (j = 0; j < stack[i].size(); j++)
				{
					result[counter] = res[j];
					counter++;
				}
			}
		}
	}
	return result;
}

double* TBBRadixSortMSD(const double* array, const uint len, uint precision, uint radix)
{
	double* result = TBBGetMemoryPool()->TBBAlloc(len);

	stack<double> stackNeg[NUM_VAL];
	stack<double> stackPos[NUM_VAL];

	int integ[1];
	double fract[1];
	int grainsize = 2;
	uint count = TBBGetMaxCountOfDigits(array, len, integ, fract);
	double val = 0;

	parallel_for(blocked_range<int>(0, (int)len, (size_t)grainsize), TBBStackDistribution(array, len, stackPos,
		stackNeg, radix, count));
	//for (uint i = 0; i < 10; ++i)
	//{
	//	while (!stackPos[i].empty())
	//	{
	//		cout << i << ": " << stackPos[i].top() << endl;
	//		stackPos[i].pop();
	//	}
	//}
	uint counter = 0;
	//int j = 0;
	//for (int i = NUM_VAL - 1; i >= 0; --i)
	//{
	//	double* res = TBBRadixSortMSDStack(count, stackPos[i], precision, radix + 1);
	//	if (NULL != res)
	//	{
	//		for (j = 0; j < (int)stackPos[i].size(); j++)
	//		{
	//			result[counter] = res[j];
	//			counter++;
	//		}
	//	}
	//}
	//for (int i = 0; i < NUM_VAL; ++i)
	//{
	//	double* res = TBBRadixSortMSDStack(count, stackNeg[i], precision, radix + 1);
	//	if (NULL != res)
	//	{
	//		for (j = (int)stackNeg[i].size() - 1; j >= 1; --j)
	//		{
	//			result[counter] = res[j] * (-1);
	//			counter++;
	//		}
	//		result[counter] = res[0] * (-1);
	//		counter++;
	//	}
	//}
	parallel_for(blocked_range<int>(0, (int)(NUM_VAL - 1), (size_t)grainsize), TBBSortMainLoop(result, len, stackPos,
		stackNeg, radix, count, counter, precision, 0));
	parallel_for(blocked_range<int>(0, (int)NUM_VAL, (size_t)grainsize), TBBSortMainLoop(result, len, stackPos,
		stackNeg, radix, count, counter, precision, 1));
	return result;
}

