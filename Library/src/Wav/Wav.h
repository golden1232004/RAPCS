/*
    *******************  CLASS - Wav ******************* 
    
    This is the standard Wav class for reading in data from 
    a wav file. Other implementations (.mp3, .mp4 etc..) can 
    be used but must be inherited from "Audio".
    

    Description ("readwav")

    The function "readwav" will read in data from a .wav file
    in two different Formats:

        Native:

        
        DOUBLE:


*/


#include <iostream>
#include <vector>
#include <fstream>
//#include "types.h"
using namespace std;

#ifndef _Wav_h
#define _Wav_h

enum ReadType  {
     NATIVE = 0, DOUBLE,
};


class Wav {

    public:

        Wav();
        Wav(const int N, const int M);

        bool readwav(string theFile, ReadType type);

        bool readHeader(ifstream &file);

        vector<double> returnRawSignal();

        int returnSampleRate();

    private:

        #pragma pack(push, 1)
        struct s_riff_hdr
        {
            char id[4];
            uint32_t size;
            char type[4];
        };

        struct s_chunk_hdr
        {
            char id[4];
            uint32_t size;
        };

        struct s_wavefmt
        {
            uint16_t format_tag;
            uint16_t channels;
            uint32_t sample_rate;
            uint32_t avg_bytes_sec;
            uint16_t block_align;
        };

        struct s_wavefmtex
        {
            s_wavefmt fmt;
            uint16_t bits_per_sample;
            uint16_t extra_size;
        };

        struct s_pcmwavefmt
        {
           s_wavefmt fmt;
           uint16_t bits_per_sample;
        };

        #pragma pack(pop) 

        char* data;
        vector<double> rawSignal;
        int sample_rate;
};

#endif