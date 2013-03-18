#include <vector>
#include <iostream>

using namespace std;

#ifndef _FFT_H_
#define _FFT_H_

#include "complex_simple.h"

vector<complex> DFT_native_1(vector<complex> &x, int N);
vector<complex> DFT_native_2(vector<complex> &x, int N);
vector<complex> FFT_simple(vector<complex> &x, int N);

#endif /* #ifndef _FFT_H_ */