#include "numeric.h"

/**
 * Allocate array of double numver and initialize them by
 * random value
 * @param min The minimum value of number
 * @param max The maximum value of number
 * @param num The numbers of double number
 * @return The pointer to array of double number  
 */
double* TBBNumRandomGenerate(double min, double max, uint num)
{
  if (0 >= num)
  {
    return NULL;
  }
  srand((unsigned int)time(0));
  double* result = TBBGetMemoryPool()->TBBAlloc(num);
#pragma omp parallel for schedule(static)
  for (int i = 0; i < num; ++i)
  {
    /*int this_thread = omp_get_thread_num(), num_threads = omp_get_num_threads();*/
    result[i] = TBBRand(min, max);
  }
  return result;  
}

/**
 * Generate random number from min value to max value
 * @param min The minimum value of number
 * @param max The maximum value of number
 * @return The double number  
 */
double TBBRand(double min, double max)
{
    double f = (double)rand() / RAND_MAX;
    return min + f * (max - min);
}
