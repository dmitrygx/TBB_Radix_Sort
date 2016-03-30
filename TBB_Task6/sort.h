#ifndef SORT_H
#define SORT_H

#include <iostream>
#include <cstdlib>
#include <omp.h>
#include <stack>
#include <cmath>
#include "numeric.h"
#include "util.h"
#include "tbbclasses.h"

#ifndef CHUNK 
#define CHUNK 10
#endif

#define NUM_VAL 10

using namespace std;


double* TBBRadixSortMSD(const double* array, const uint len, uint precision, uint radix);

#endif
