#ifndef NUMERIC_H
#define NUMERIC_H

#include <iostream>
#include <cstdlib>
#include <omp.h>
#include <ctime>
#include "util.h"

using namespace std;

#define uint unsigned long long int

double* TBBNumRandomGenerate(double min, double max, unsigned long long int num);

double TBBRand(double min, double max);

#endif
