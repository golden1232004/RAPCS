#include <fstream>
#include <iomanip>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <algorithm>
#include <math.h>
#include <map>
#include "includes/audiolib.h"

using namespace std;

map<string, string> params;

bool exists(string const& param, const map<string, string> &params)
{
    return (params.find(param) != params.end());
}

int main(int argc, char* argv[])
{
    if(argc <= 1)
    {
        cout << "Error";
        exit(0);
    }
    bool waitingForParam = true;
    string paramName;
    
    for (int i = 1; i < argc; ++i) {
        if (waitingForParam) {
            if (argv[i][0] == '-') { // new param 
                paramName = argv[i];
                waitingForParam = false;
            }
        }
        else {
            if (argv[i][0] == '-') { // empty value
                params.insert(make_pair(paramName, string()));
                paramName = argv[i];
            } else {
                params.insert(make_pair(paramName, string(argv[i])));
                waitingForParam = true;
            }
        }
    }
    
    string sample_name = argv[1];
    Wav sampleWave;
    MFCC mfcc;

    if(!sampleWave.readwav(sample_name, DOUBLE))
    {
        cout << "Cannot read the sample file";
        exit(0);
    }

    vector<double> rawData = sampleWave.returnRawSignal();

    int sample_rate = sampleWave.returnSampleRate();

    vector<DIMENSIONS_2> MFCC = mfcc.transform(rawData, sample_rate);

    if(exists("-t", params))
    {   
        Training training("repository/training/");
        if(training.open())
        {
            struct sampleData sample;

            sample.N = MFCC.size();
            sample.M = MFCC[0].size();
            sample.sample_name = argv[3];
            sample.speaker = argv[4];

            training.write_2D(MFCC, sample);
        }
    }


    if(exists("-r", params))
    {
        Training training("repository/training/");
        pair<int, string> minDistance(INT_MAX, "");
        if(training.open())
        {
            vector<string> files = training.files();
            string person; 

            for(unsigned i=0; (i < files.size()); i++)
            {
                vector<vector<double> > data;
                data.clear();
                string* speaker_features = new string[2];

                data = training.read_2D("repository/training/" + files[i], speaker_features);

                int currDistance = DTW::Distance(MFCC, data);
                if(currDistance < minDistance.first) {
                    minDistance.first = currDistance;
                    minDistance.second = speaker_features[0];
                    person = speaker_features[1];

                }
            }
            cout << "You said: " << minDistance.second << " You sound like: " << person << endl;
    
      }else{
        cout << "Cannot open the training file";
      }
    }

}