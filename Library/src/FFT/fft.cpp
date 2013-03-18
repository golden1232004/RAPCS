#include "fft.h"
#include <stdlib.h>
#include <vector>
#include <iostream>
#include <math.h>
using namespace std;

#define PI  3.1415926535897932

vector<complex> DFT_native_1(vector<complex> &x, int N)
{
	vector<complex> X(N);

	for(unsigned i=0; (i < N); i++)
	{
		X[i].re = X[i].im = 0.0;
		for(unsigned n = 0; (n < N); n++)
		{
			X[i] = complex_add(X[i], complex_mult(x[n], complex_from_polar(1, -2*PI*n*i/N)));
		}
	}

	return X;
}
vector<complex> DFT_native_2(vector<complex> &x, int N)
{
	vector<complex> X(N);
	vector<complex> Nth_root(N);

	int k, n;
    for(k=0; k<N; k++) {
        Nth_root[k] = complex_from_polar(1, -2*PI*k/N);
    }
    for(k=0; k<N; k++) {
        X[k].re = X[k].im = 0.0;
        for(n=0; n<N; n++) {
            X[k] = complex_add(X[k], complex_mult(x[n], Nth_root[(n*k) % N]));
        }
    }
    return X;
}
vector<complex> FFT_simple(vector<complex> &x, int N)
{
	vector<complex> X(N);

	vector<complex> d;
	vector<complex> e;
	vector<complex> D;
	vector<complex> E;

	 int k;

    if (N == 1) {
        X[0] = x[0];
        return X;
    }

    e.resize(N/2);
    d.resize(N/2);

    //e = (complex*) malloc(sizeof(struct complex_t) * N/2);
    //d = (complex*) malloc(sizeof(struct complex_t) * N/2);
    for(k = 0; k < N/2; k++) {
        e[k] = x[2*k];
        d[k] = x[2*k + 1];
    }

    E = FFT_simple(e, N/2);
    D = FFT_simple(d, N/2);

    //free(e);
    //free(d);

    for(k = 0; k < N/2; k++) {
        /* Multiply entries of D by the twiddle factors e^(-2*pi*i/N * k) */
        D[k] = complex_mult(complex_from_polar(1, -2.0*PI*k/N), D[k]);
    }

    for(k = 0; k < N/2; k++) {
        X[k]       = complex_add(E[k], D[k]);
        X[k + N/2] = complex_sub(E[k], D[k]);
    }


    return X;

}