#include <iostream>
#include <vector>
#include <math.h>

#include "MFCC.h"
#include "../FFT/fft.h"

using namespace std;

typedef vector<double> DIMENSIONS_2;
typedef vector<complex> complex_di2;
#define EXPORT __attribute__((visibility("default")))

const double PI  = 3.141592653589793238462;

EXPORT
MFCC::MFCC() {

	this->mel.resize(16);
	this->melstart.resize(16);
	this->mellength.resize(16);

}
EXPORT
MFCC::MFCC(unsigned int i, unsigned j)
{


}

void printComplexFrame(vector<complex> &frame)
{
	for(int i=0; (i < frame.size()); i++)
	{
		cout << frame[i].im  << endl;
	}

}
void printFrame(vector<double> &frame)
{
	for(int i=0; (i < frame.size()); i++)
	{
		cout << frame[i] << endl; 
	}
}

vector<complex_di2> convertToComplex(vector<DIMENSIONS_2> &frames, int fft_size)
{	
	vector<complex_di2> temp_frames;

	temp_frames.resize(frames.size());
	for(unsigned i=0; (i < frames.size()); i++)
	{	
		temp_frames[i].resize(fft_size);

		for(unsigned j=0; (j < fft_size); j++)
		{
			temp_frames[i][j].re = (double) frames[i][j];
			temp_frames[i][j].im = (double) frames[i][j];
		}
	}
	return temp_frames;
}
EXPORT
vector<DIMENSIONS_2> MFCC::transform(vector<double> &signal, int something)
{	
	int M = 100;
	int N = 256;	
	double max = 0;
	int fft_size = N;

	this->filter(signal, 0.95);
	vector<DIMENSIONS_2> frames = this->frame(signal, N, M);
	
	this->hamming_window(frames, max);

	// ******* SET-UP MEL FRAME *********** // 

	//vector<int> melstart(16, 0);
	//vector<int> mellength(16, 0);
	//vector<DIMENSIONS_2> mel(16, 0); // Initalise vector for 

	this->mel_frame(fft_size, something);

	vector<complex_di2> FFT_VARS = convertToComplex(frames, fft_size);

	vector<DIMENSIONS_2> MFCC(frames.size());

	for(unsigned i=0; (i < frames.size()); i++)
	{
		vector<complex> FFT = FFT_simple(FFT_VARS[i], fft_size);
		vector<double> temp_mfcc = this->calculateMFCC(FFT, fft_size);

		for(int j=0; (j < temp_mfcc.size()); j++)
		{
			MFCC[i].resize(temp_mfcc.size());

			MFCC[i][j] = temp_mfcc[j];
		}
	}
	// ******* END MEL FRAME *********** // 


	// ******* SET-UP FFT *********** // 

	return MFCC;
}
EXPORT
vector<double> DCT(vector<double> &x, int N)
{
		vector<double> X(N);
		int k,n;
        
        for(k = 0; k < N; k++) 
        {
        X[k] = 0.0;
        
        for(n = 0; n < N; n++) 
        {
            X[k] = (X[k] + (x[n]*cos((PI/N)*(n-.5)*k)));
        }
    }
    return X;
}
EXPORT
vector<double> MFCC::calculateMFCC(vector<complex> &FFT, int fft_size)
{	

	vector<double> value(fft_size*2);

	for(int i=0; (i < fft_size); i++)
	{
		value[i] = fabs(FFT[i].re + FFT[fft_size - i - 1].re + FFT[i].im - FFT[fft_size - i - 1].im)/2;
	}

	for(int i=0; (i < fft_size); i++)
	{
		value[i+fft_size] = fabs(FFT[i].re + FFT[i].im + FFT[fft_size - i - 1].im - FFT[fft_size - i - 1].re)/2;
	}

	for (int i=0; i<fft_size/2; i++) 
	{    
        value[i] += value[fft_size - i - 1];
        value[fft_size + i] += value[2*fft_size - i - 1];
    }

	vector<double> result(13, 0);
	for(int i=0; (i < 13); i++)
    {
        for(int j=0; (j < mellength[i]); j++)
        {
            result[i] += mel[i][j] * value[j+melstart[i]];
        }

        result[i] = log(result[i]);
    }

    result = DCT(result, 13);

    return result;
}
EXPORT
void MFCC::mel_frame(int fft_size, int sampleRate)
{

	double fmax;
    double dphi;
    double fsample;
    double freq;
    double temp[fft_size/2];

    fmax = 2595*log10(8000.0f/700+1);
    dphi = fmax / 17;
    freq = (double)sampleRate/fft_size;

      for(int i=0; (i < 16); i++)
      { 
           melstart[i]=fft_size/2;
           mellength[i]=0;
           memset(temp,0,sizeof(double)*fft_size/2);


          for(int j=0; (j < fft_size/2); j++)
          {
              mel[i].reserve(mellength[i]);
              fsample = 2595*log10(freq*j/700 + 1);
              if ((dphi*i <= fsample) && (fsample < dphi*(i+1))) temp[j] = (fsample-dphi*i)/(dphi*(i+1)-dphi*i);
                     
			   if ((dphi*(i+1) <= fsample) && (fsample < dphi*(i+2))) temp[j] = (fsample-dphi*(i+2))/(dphi*(i+1)-dphi*(i+2));
                        if ((temp[j] != 0) && (melstart[i] > j)) melstart[i] = j;
                        if (temp[j] != 0) mellength[i]++;


          }
          
            mel[i].reserve(mellength[i]);
            mel[i].assign(&temp[melstart[i]], &temp[melstart[i]] + mellength[i]);
      }
}

EXPORT
void MFCC::filter(vector<double> &signal, double filterPoint) 
{
	for(unsigned i=1; (i < signal.size()-1); i++)
	{
		signal[i] = signal[i] - (0.95 * signal[i-1]);
	}
}
EXPORT
vector<DIMENSIONS_2> MFCC::frame(vector<double> &signal, int N, int M)
{
	 unsigned int n = signal.size();
	 unsigned int num_blocks = n / N;

	
	 unsigned int maxblockstart = n - N;
	 unsigned int lastblockstart = maxblockstart - (maxblockstart % M);
	 unsigned int numbblocks = (lastblockstart)/M + 1;


	 this->width = N;
	 this->height = numbblocks;

	 vector<DIMENSIONS_2> blocked(numbblocks);

	 for(unsigned i=0; (i < numbblocks); i++)
	 {
	 	blocked[i].resize(N);
        
        for(int j=0; (j < N); j++)
        {
            blocked[i][j] = signal[i*M+j];
        }
	 }

	 return blocked;

}
EXPORT
vector<double> MFCC::get_hamming_frame(vector<double> &frame)
{	
	vector<double> hamming_window_result;

	for(int i=0; (i < frame.size()); i++)
	{
		double value = 0.54 - 0.46 * cos(2 * PI * i / (frame.size() - 1));
		hamming_window_result.push_back(value);
	}

	return hamming_window_result;
}
EXPORT
void MFCC::hamming_window(vector<DIMENSIONS_2> &frames, double &max)
{
	vector<double> hamming_res = this->get_hamming_frame(frames[0]);
	vector<double>::iterator result;

	result = max_element(hamming_res.begin(), hamming_res.end());

	max = *result;

	for(unsigned i=0; (i < frames.size()); i++)
	{
		for(unsigned j=0; (j < frames[i].size()); j++)
		{
			frames[i][j] = frames[i][j] * hamming_res[j];
		}
	}
}




