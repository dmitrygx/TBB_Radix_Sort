#ifndef SORT_H
#define SORT_H

#include <iostream>
#include <cstdlib>
#include <omp.h>
#include <stack>
#include <cmath>
#include "numeric.h"
#include "util.h"

#ifndef CHUNK 
#define CHUNK 10
#endif

#define NUM_VAL 2

using namespace std;

double* TBBRadixSortMSD(const double* array, const uint len, uint radix);

double* TBBMSDRadixSort(const double* array, const uint len, uint radix, uint full, double *result);

void TBBAddNewElemToAuxArr(double elem, double *arr, unsigned int bit, uint *left0, uint *right1);

#endif
