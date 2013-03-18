//
//  Wav.cpp
//
//
//  Created by Phillip Heels on 24/09/2012.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <vector>
#include <inttypes.h>
#include <stdint.h>
#include <math.h>
#include "Wav.h"
#include "types.h"

using namespace std;
#define EXPORT __attribute__((visibility("default")))

bool readHeader(ifstream &file);

int size = 0;
int bits_per_sample;
int sample_rate2;

EXPORT
Wav::Wav(){};
EXPORT
Wav::Wav(const int N, const int M)
{
    
}
EXPORT
bool Wav::readwav(string theFile, ReadType type)
{
    // Attempt to open the .wav file
    ifstream file (theFile.c_str());
    
    if(!readHeader(file))
    {
        return 0;
    }

   // const int foo = bits_per_sample;

    IntN< 8>::type x;

    switch(type)
    {
        case NATIVE:

            /*for(unsigned i=0; (i < size); i++)
            {
                c = (unsigned)(unsigned char) (data[i]);
                this->rawSignal.push_back(c);
            }
            */
        break;

        case DOUBLE:
            sample_rate = sample_rate2;
            if(bits_per_sample == 8)
            {
                uint8_t c;

                for(unsigned i=0; (i < size); i++)
                {
                    c = (unsigned)(unsigned char)(data[i]);
                    double t = (c-128)/128.0;
                    this->rawSignal.push_back(t);
                }
            }
            else if(bits_per_sample == 16)
            {
                uint8_t c;

                for(unsigned i=0; (i < size); i++)
                {
                    c = (signed)(signed char)(data[i]);
                    double t = (c-256)/256.0;
                    this->rawSignal.push_back(t);
                }
            }else{
               cerr << "Cannot Initalise correct 'bits_per_sample' in reference to function: 'readWav' line: 67";

            }


            break;
            
        default:
            return false;
            break;
    }

    return true;
    
}
EXPORT
bool Wav::readHeader(ifstream &file)
{   
    s_riff_hdr riff_hdr;
    s_chunk_hdr chunk_hdr;

    uint32_t padded_size; // Size of extra bits

    vector<uint8_t> fmt_data; // Vector to store the FMT data.

    s_wavefmt *fmt = NULL;

    file.read(reinterpret_cast<char*>(&riff_hdr), sizeof(riff_hdr));
    if (!file) return false;

    if (memcmp(riff_hdr.id, "RIFF", 4) != 0) return false;

    //cout << "size=" << riff_hdr.size << endl;
    //cout << "type=" << string(riff_hdr.type, 4) << endl;

    if (memcmp(riff_hdr.type, "WAVE", 4) != 0) return false;
    {
         do
    {
        file.read(reinterpret_cast<char*>(&chunk_hdr), sizeof(chunk_hdr));
        if (!file) return false;

        padded_size = ((chunk_hdr.size + 1) & ~1);

        //cout << "id=" << string(chunk_hdr.id, 4) << endl;
        //cout << "size=" << chunk_hdr.size << endl;
        //cout << "padded size=" << padded_size << endl;

        if (memcmp(chunk_hdr.id, "fmt ", 4) == 0) 
        {
            if (chunk_hdr.size < sizeof(s_wavefmt)) return false;

            fmt_data.resize(padded_size);
            file.read(reinterpret_cast<char*>(&fmt_data[0]), padded_size);
            if (!file) return false;

            fmt = reinterpret_cast<s_wavefmt*>(&fmt_data[0]);

            //cout << "format_tag=" << fmt->format_tag << endl;
            //cout << "channels=" << fmt->channels << endl;
            //cout << "sample_rate=" << fmt->sample_rate << endl;
            sample_rate2 = fmt->sample_rate;

            ///cout << sample_rate;
            //cout << "avg_bytes_sec=" << fmt->avg_bytes_sec << endl;
            //cout << "block_align=" << fmt->block_align << endl;
           // this->sample_rate = fmt->sample_rate;
            if (fmt->format_tag == 1) // PCM
            {
                if (chunk_hdr.size < sizeof(s_pcmwavefmt)) return false;

                s_pcmwavefmt *pcm_fmt = reinterpret_cast<s_pcmwavefmt*>(fmt);

                //cout << "bits_per_sample=" << pcm_fmt->bits_per_sample << endl;
                bits_per_sample = pcm_fmt->bits_per_sample;
            }
            else
            {
                if (chunk_hdr.size < sizeof(s_wavefmtex)) return false;

                s_wavefmtex *fmt_ex = reinterpret_cast<s_wavefmtex*>(fmt);

                //cout << "bits_per_sample=" << fmt_ex->bits_per_sample << endl;
                //cout << "extra_size=" << fmt_ex->extra_size << endl;

                if (fmt_ex->extra_size != 0)
                {
                    if (chunk_hdr.size < (sizeof(s_wavefmtex) + fmt_ex->extra_size)) return false;

                    uint8_t *extra_data = reinterpret_cast<uint8_t*>(fmt_ex + 1);
                    // use extra_data, up to extra_size bytes, as needed...
                }
            }
        }
        else if (memcmp(chunk_hdr.id, "data", 4) == 0)
        {
            // process chunk data, according to fmt, as needed...
            size = padded_size;
            data = new char[padded_size];
            
            file.read(data,     padded_size);
                
            file.ignore(padded_size);
            if (!file) return false;
        }
        else
        {
            // process other chunks as needed...

            file.ignore(padded_size);
            if (!file) return false;
        }

    }while (!file.eof());
    
    return true;
}
}
EXPORT
vector<double> Wav::returnRawSignal() {

    return this->rawSignal;
}
EXPORT
int Wav::returnSampleRate() {
    return this->sample_rate;
}

