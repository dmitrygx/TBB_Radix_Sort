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

	setCountOfThreads(4);

	task_scheduler_init init(getCountOfThreads());
	(void)TBBParseArgs(argc, argv, min, max, num, precision);
	TBBInitMemoryPool(num * 64);
	double* array = TBBNumRandomGenerate(min, max, num);
	if (NULL == array)
	{
		cout << "Random array hasn't been obtained" << endl;
		return -1;
	}

	TBBOutput(true, "tbb_array.txt", "Source array:", array, num, precision);
	time_t start = clock();
	double *result = TBBGetMemoryPool()->TBBAlloc(num);
	TBBMSDRadixSort1& tbbSort = *new(task::allocate_root()) TBBMSDRadixSort1(array, num, 0, num, result);
	task::spawn_root_and_wait(tbbSort);
	time_t end = clock();
	time_t diff = end - start;
	cout << "Time: " << ((double)diff) / CLOCKS_PER_SEC << " sec." << endl;
	if (NULL == result)
	{
		cout << "Array hasn't been sorted" << endl;
		return -1;
	}
	TBBOutput(true, "tbb_result.txt", "Result of array sorting:", result, num, precision);
	TBBGetMemoryPool()->TBBFree(num, result);
	TBBTerminateMemoryPool();

	return 0;
}
