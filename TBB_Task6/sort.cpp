#include "sort.h"

double* TBBRadixSortMSDStack(stack<double> st, uint radix)
{
	double* result;
	stack<double> stack[NUM_VAL];
	uint thr1 = 0;
	uint thr0 = 0;
	if (st.empty())
	{
		return NULL;
	}
	else
	{
		result = TBBGetMemoryPool()->TBBAlloc(st.size());
		if ((64 == radix) || (st.size() == 1))
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
		(TBBGetBit(value, radix)) == 0 ? thr0++ : thr1++;
		stack[TBBGetBit(value, radix)].push(value);
	}
	uint counter = 0;
	uint counter1 = 0;
	int i;
	double* res1;
	double* res2;
	//#pragma omp parallel shared(stack) num_threads(2)

	//#pragma omp sections
#pragma omp parallel shared(stack) num_threads(2)
	{
#pragma omp sections
	{
#pragma omp section
	{
		{
			i = 1;
			res1 = TBBRadixSortMSDStack(stack[i], radix + 1);
			if (NULL != res1)
			{
				for (int j = 0; j < (int)stack[i].size(); j++)
				{
					{
						result[counter] = res1[j];
						counter++;
					}
				}
			}
		}
	}
#pragma omp section
	{
		{
			int i1 = 0;
			res2 = TBBRadixSortMSDStack(stack[i1], radix + 1);
			if (NULL != res2)
			{
				for (int j = 0; j < (int)stack[i1].size(); j++)
				{
					{
						result[thr1 + counter1] = res2[j];
						counter1++;
					}
				}
			}
		}
	}
	}
	}
	TBBGetMemoryPool()->TBBFree(thr1, res1);
	TBBGetMemoryPool()->TBBFree(thr0, res2);
	return result;
}

double* TBBRadixSortMSD(const double* array, const uint len, uint radix)
{
	double* result;
		result = TBBGetMemoryPool()->TBBAlloc(len);
	stack<double> stack[NUM_VAL];
	uint thr1 = 0;
	uint thr0 = 0;
	double val = 0;
		for (int i = 0; i < (int)len; ++i)
		{
				val = array[i];
				(TBBGetBit(val, radix)) == 0 ? thr0++ : thr1++;
				stack[TBBGetBit(val, radix)].push(val);
		}
	uint counter1 = 0;
	uint counter2 = 0;
	int j = 0;
	double* res1;
	double* res2;
	res1 = TBBRadixSortMSDStack(stack[0], radix + 1);
		if (NULL != res1)
		{
				for (j = 0; j < (int)thr0; j++)
				{
					result[counter1] = res1[j];
					counter1++;
				}
		}
		res2 = TBBRadixSortMSDStack(stack[1], radix + 1);
		if (NULL != res2)
		{
					for (j = (int)thr1 - 1; j >= 0; --j)
					{
						result[thr0 + counter2] = res2[j];
						counter2++;
					}
		}
	return result;
}

void TBBAddNewElemToAuxArr(double elem, double *arr, unsigned int bit, uint *left0, uint *right1)
{
	if (!bit)
	{
		arr[*left0] = elem;
		(*left0)++;
	}
	else
	{
		(*right1)--;
		arr[*right1] = elem;
	}
}

double* TBBMSDRadixSort(const double* array, const uint len, uint radix, uint full)
{
	double *result;
	double *aux;
	uint thr1 = 0;
	uint thr0 = 0;
	double val = 0;
	unsigned int bit = 0;
	uint left0 = 0;
	uint right1 = len;
	if (len == 0)
	{
		return NULL;
	}
	else
	{
		if ((64 == radix) || (len == 1))
		{
			result = TBBGetMemoryPool()->TBBAlloc(len);
			uint counter = 0;
			for (int i = 0; i < (int)len; ++i)
			{
				result[counter] = array[i];
				counter++;
			}
			return result;
		}
	}
	result = TBBGetMemoryPool()->TBBAlloc(len);
	aux = TBBGetMemoryPool()->TBBAlloc(len);
	for (int i = 0; i < (int)len; ++i)
	{
		val = array[i];
		bit = TBBGetBit(val, radix);
		TBBAddNewElemToAuxArr(val, aux, bit, &left0, &right1);
		(!bit) ? thr0++ : thr1++;
	}
	uint counter1 = 0;
	uint counter2 = 0;
	double* res1;
	double* res2;
	if (len >= 10000)
	{

		res2 = TBBMSDRadixSort(aux + thr0, thr1, radix + 1, full);
		if (NULL != res2)
		{
			if (0 == radix)
			{
				for (int j = (int)thr1 - 1; j >= 0; --j)
				{
					result[counter2] = res2[j];
					counter2++;
				}
			}
			else
			{
				for (int j = 0; j < (int)thr1; j++)
				{
					result[thr0 + counter2] = res2[j];
					counter2++;
				}
			}
			TBBGetMemoryPool()->TBBFree(thr0, res2);
		}
		res1 = TBBMSDRadixSort(aux, thr0, radix + 1, full);
		if (NULL != res1)
		{
			if (0 == radix)
			{
				for (int j = 0; j < (int)thr0; j++)
				{
					result[thr1 + counter1] = res1[j];
					counter1++;
				}
			}
			else
			{
				for (int j = 0; j < (int)thr0; j++)
				{
					result[counter1] = res1[j];
					counter1++;
				}
			}
			TBBGetMemoryPool()->TBBFree(thr1, res1);
		}
	}
	else
	{
		res2 = TBBMSDRadixSort(aux + thr0, thr1, radix + 1, full);
		if (NULL != res2)
		{
			if (0 == radix)
			{
				for (int j = (int)thr1 - 1; j >= 0; --j)
				{
					result[counter2] = res2[j];
					counter2++;
				}
			}
			else
			{
				for (int j = 0; j < (int)thr1; j++)
				{
					result[thr0 + counter2] = res2[j];
					counter2++;
				}
			}
			TBBGetMemoryPool()->TBBFree(thr0, res2);
		}
		res1 = TBBMSDRadixSort(aux, thr0, radix + 1, full);
		if (NULL != res1)
		{
			if (0 == radix)
			{
				for (int j = 0; j < (int)thr0; j++)
				{
					result[thr1 + counter1] = res1[j];
					counter1++;
				}
			}
			else
			{
				for (int j = 0; j < (int)thr0; j++)
				{
					result[counter1] = res1[j];
					counter1++;
				}
			}
			TBBGetMemoryPool()->TBBFree(thr1, res1);
		}
	}
	TBBGetMemoryPool()->TBBFree(len, aux);
	return result;
}