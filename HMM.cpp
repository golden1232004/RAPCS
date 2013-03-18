#include <fstream>
#include <iomanip>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <algorithm>
#include <math.h>
#include "includes/audiolib.h"
using namespace std;

/* 
	@
	@
	@ / setupHMM
	  // Created: March 12 2013
	@ Author: Phillip Heels

	@notes:

	  This function converts the vector (2D) into an array
	  This is to allow for the (temp) implementation of the
	  Hidden Markov Model (C). Do not use this function
	  unless the functions for the HMM handle arrays. 
*/


double** setupHMM(vector<vector<double> > &vals, int N, int M)
{
   double** temp;

   temp = new double*[N];

   for(unsigned i=0; (i < N); i++)
   {
       temp[i] = new double[M];
       for(unsigned j=0; (j < M); j++)
       {
           temp[i][j] = vals[i][j];
       }
   }
   return temp;
}

int main() {
	
	Training trainingNO("repositry/training/HMM_NO");
	model_t* hmm_no;
	model_t* hmm_yes;
	if(trainingNO.open())
	{
		vector<vector<double> > training_no;
		string* speaker_features = new string[2];
		training_no = trainingNO.read_2D("repositry/training/HMM_NO/training-1.txt", speaker_features);
		double** convertNO = setupHMM(training_no, training_no.size(), 13);
		hmm_no = hmm_init(convertNO, training_no.size(), 13, 1);
	}

	Training trainingYES("repositry/training/HMM_YES");
	if(trainingYES.open())
	{
		vector<vector<double> > training_yes;
		string* speaker_features1 = new string[2];
		training_yes = trainingYES.read_2D("repositry/training/HMM_YES/training-1.txt", speaker_features1);
		double** convertYES = setupHMM(training_yes, training_yes.size(), 13);
		hmm_yes = hmm_init(convertYES, training_yes.size(), 13, 1);
	}

	Training trainingSTOP("repositry/training/HMM_STOP");
	if(trainingYES.open())
	{
		vector<vector<double> > training_stop;
		string* speaker_features2 = new string[2];
		training_stop = trainingSTOP.read_2D("repositry/training/HMM_STOP/training-1.txt", speaker_features2);
		double** convertStop = setupHMM(training_stop, training_stop.size(), 13);
		hmm_yes = hmm_init(convertStop, training_stop.size(), 13, 1);
	}
	/* Set up the training files */ 

	Training training ("repositry/training/");

	if(training.open())
	{
		vector<string> files = training.files();

		for(unsigned i=0; (i < files.size()); i++)
		{
			vector<vector<double> > training_data;
			training_data.clear();

			string* speaker_features = new string[2];

			training_data = training.read_2D("repositry/training/" + files[i], speaker_features);

			double** convertTraining = setupHMM(training_data, training_data.size(), 13);

			if(speaker_features[0] == "Yes")
			{
				hmm_train(convertTraining, training_data.size(), hmm_yes);
			}else if(speaker_features[0] == "No")
			{
				hmm_train(convertTraining, training_data.size(), hmm_no);
			}else if(speaker_features[0] == "Stop")
			{
				
			}
		}
	}

	Wav sampleWave;
    MFCC mfcc;
    string file = "repositry/jonno.wav";
    if(!sampleWave.readwav("repositry/stop.wav", DOUBLE))
    {
        cerr << "We cannot find the file: " << file << endl;
        exit(0);   
    }

    vector<double> rawData = sampleWave.returnRawSignal();

    int sample_rate = sampleWave.returnSampleRate();

    vector<DIMENSIONS_2> MFCC = mfcc.transform(rawData, sample_rate);

    double** sample = setupHMM(MFCC, MFCC.size(), 13);
    double yes;
    double no;
   
    int* b = new int[MFCC.size()];
	int* q = new int[MFCC.size()];
      		
  
    viterbi_decode(sample, MFCC.size(), hmm_yes, q, yes);

    viterbi_decode(sample, MFCC.size(), hmm_no, q, no);

    if(yes > no)
    {
    	cout << "Output: Yes";
    }else if(no > yes)
    {
    	cout << "Output: No";
    }else{
    	cout << "Output: Stop";
    }
 	

}