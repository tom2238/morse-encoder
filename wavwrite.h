/* ========================================================================== */
/*                                                                            */
/*   wavwrite.h                                                               */
/*   (c) 2017 Tomas Dubina                                                    */
/*                                                                            */
/*   Write audio to wav file                                                  */
/*                                                                            */
/* ========================================================================== */
#ifndef _WAV_WRITE_MORSE
#define _WAV_WRITE_MORSE

#define  WF_TPI   6.2831853 // 2xPI
#define  WF_SRATE 16000     // fixed sample rate 16 kHz
#define  WF_CHAN  1         // one channel = mono
#define  WF_BITS  16        // bits per sample
//put your function headers here
//Write Integer value to wav file
int fwrite_int(int val, char len, FILE *p);
//Write wav file header
int wav_head(double tdur, unsigned int fs,unsigned int bits, FILE *filename);
//write sine wave, return phase
double wav_sine(unsigned int freq, unsigned int numsp, unsigned int sample_rate,double phase, FILE *zp);
//write space (no sound)
int wav_space(unsigned int numsp, FILE *zp);
#endif
