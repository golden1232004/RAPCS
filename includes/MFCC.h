
#include <iostream>
#include <vector>
#include "fft.h"
#ifndef _MFCC_h
#define _MFCC_h

using namespace std;

typedef vector<double> DIMENSIONS_2;

class MFCC {

	public:

		MFCC();
		MFCC(unsigned int i, unsigned j);

		vector<DIMENSIONS_2> transform(vector<double> &signal, int FrameSample);
		void filter(vector<double> &signal, double filterPoint);
		vector<DIMENSIONS_2> frame(vector<double> &signal, int N, int M);
		void mel_frame(int fft_size, int sampleRate);
		vector<double> get_hamming_frame(vector<double> &frame);
		void hamming_window(vector<DIMENSIONS_2> &frames, double &max);
		vector<double> calculateMFCC(vector<complex> &fft_vect, int fft_size);

	 public:

		vector<DIMENSIONS_2> mel;
		vector<int> melstart;
		vector<int> mellength;
		
		int width;
		int height;
};
#endif