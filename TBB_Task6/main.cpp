#include "tbb/task_scheduler_init.h"
#include "tbb/parallel_for.h"
#include "tbb/blocked_range.h"

#include <ctime>

#include "main.h"

using namespace tbb;
using namespace std;

int main(int argc, char **argv)
{
	double min, max;
	uint num, precision;
	task_scheduler_init init;

	(void)TBBParseArgs(argc, argv, min, max, num, precision);

	TBBInitMemoryPool(num * 20);

	double* array = TBBNumRandomGenerate(min, max, num);
	double* result;
	if (NULL == array)
	{
		cout << "Random array hasn't been obtained" << endl;
		return -1;
	}

	TBBOutput(true, "omp_array.txt", "Source array:", array, num);
	time_t start = clock();
	result = TBBRadixSortMSD(array, num, precision, 1);
	time_t end = clock();
	time_t diff = end - start;
	cout << "Time: " << ((double)diff) / CLOCKS_PER_SEC << " sec." << endl;
	if (NULL == result)
	{
		cout << "Array hasn't been sorted" << endl;
		return -1;
	}
	TBBOutput(true, "omp_result.txt", "Result of array sorting:", result, num);

	TBBTerminateMemoryPool();

	return 0;
}
