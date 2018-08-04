/* ========================================================================== */
/*                                                                            */
/*   wavwrite.c                                                               */
/*   (c) 2017 Tomas Dubina                                                    */
/*                                                                            */
/*   Write audio to wav file                                                  */
/*                                                                            */
/* ========================================================================== */
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <time.h> 
#include "wavwrite.h"

int fwrite_int(int val, char len, FILE *p){
    unsigned int byte;
    while (len-- > 0)
    {
        byte = val & 0xFF;
        if (fwrite(&byte, 1, 1, p) != 1) {
           return 1;
        }
        val >>= 8;
    }
    return 0;
}

int wav_head(double tdur, unsigned int fs, unsigned int bits, FILE *filename){
   unsigned int numsa = (unsigned int)(tdur*fs);   // overall number of samples
   unsigned int subchunk2 = (numsa * WF_CHAN * bits)/8; //
   unsigned int blockAlign = (WF_CHAN*bits)/8;      //block size 
   unsigned int byteRate = (fs * WF_CHAN * bits)/8; //byte rate per sample
   // chunk id RIFF
   fwrite("RIFF", 4, 1, filename);
   // chunk size    
   fwrite_int(36 + subchunk2, 4, filename);
   // spec. RIFF form for WAV  
   fwrite("WAVE", 4, 1, filename);
   // subchunk1 id  format description       
   fwrite("fmt ", 4, 1, filename); 
   // subchunk1 size: 16 for PCM
   fwrite_int(16, 4, filename); 
   // audio_format: 1 = PCM
   fwrite_int(1, 2, filename);   
   // channels: mono   
   fwrite_int(WF_CHAN, 2, filename);
   // sample rate
   fwrite_int(fs, 4, filename);
   // byte rate        
   fwrite_int(byteRate, 4, filename);   
   // block align, byte rate					
   fwrite_int(blockAlign, 2, filename); 
   // bits per sample, 16 bits
   fwrite_int(bits, 2, filename);    
   // subchunk2 id  data content
   fwrite("data", 4, 1, filename); 
   // subchunk2 size
   fwrite_int(subchunk2, 4, filename);   
   return numsa;
}

double wav_sine(unsigned int freq, unsigned int numsp, unsigned int sample_rate, double phase ,FILE *zp){
  double ph=phase, deltaph;
  unsigned int sig;    
  deltaph = WF_TPI*freq/sample_rate;  // delta phase
  while(numsp-- > 0)
   {
      sig = 120*256*(cos(ph)); //aplitude cosine wave
      fwrite(&sig, 2, 1, zp);
      ph += deltaph;					//phase
   }

  return ph;
}

int wav_space(unsigned int numsp, FILE *zp){
  unsigned int num=numsp;
  unsigned int sig=0;    
  srand(time(0));   
  while(numsp-- > 0) {
      sig = rand()%256-128;
      fwrite(&sig, 2, 1, zp);
   }
  return num;
}

