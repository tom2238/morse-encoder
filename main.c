/* ========================================================================== */
/*                                                                            */
/*   main.c                                                                   */
/*   (c) 2017 Tomas Dubina                                                    */
/*                                                                            */
/*   TXT file to Morse code, write wav file                                   */
/*                                                                            */
/* ========================================================================== */
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <getopt.h>
#include <unistd.h>
#include "morse.h"
#include "wavwrite.h"
#include "buildnumber.h"
#define _CW_FILE_NO_SET "σame"

struct options {
  unsigned int freq;
  unsigned int sample_rate;
  char *file_in;
  char *file_out;
  int wpm_speed;
  int transParam;
};

void Usage(char *p_name);

int main(int argc, char *argv[])
{
  if(argc==1){
    Usage(argv[0]);
    return 0;
  }

  struct options Option = {_CW_DEF_CARRIER, WF_SRATE, _CW_FILE_NO_SET, _CW_FILE_NO_SET, _CW_DEF_SPEED, 0};
         //Sine wave frequency in Hz, default value 1000 Hz
         //Sampling frequency of wav file in Hz, default value 16000 Hz 
         //File input
         //File output
         //Speed in words per minute, default value 24 WPM
         //Extra options

  int opt=0;                      //Argument code
  char silent=0;
  char *argv_txt;
  int len;
   while ((opt = getopt(argc, argv, "hi:o:w:f:r:smgblWMRS")) != -1) {
    switch (opt) {
      case 'i': //Input file  name
        Option.file_in = optarg;
        break;
      case 'o': //Output file name
        Option.file_out = optarg;
        break;
      case 'w': //WPM
        Option.wpm_speed = atoi(optarg);
        break;
      case 'f': //Frequency
        Option.freq = strtoul(optarg, &argv_txt, 10);
        break;
      case 'r': //Sample rate
        Option.sample_rate = strtoul(optarg, &argv_txt, 10);
        break;
      case 'h': //Help
        Usage(argv[0]);
        return 0;
        break;
      case 'm': //Enable start-stop marks
        Option.transParam+=1;
        break;
      case 's': //Silent mode
        silent=1;
        break;
      case 'g': //Morse msg is OK
        Option.transParam|=1<<1;
        break;
      case 'b': //Morse msg is bad
        Option.transParam|=1<<2;
        break;
      case 'l': //Set morse speed slower
        Option.transParam|=1<<3;
        break;
      case 'W': //Please wait
        Option.transParam|=1<<4;
        break;
      case 'M': //Mistake
        Option.transParam|=1<<5;
        Option.transParam|=1<<6;
        break;
      case 'R': //Repeat message
        Option.transParam|=1<<5;
        Option.transParam|=1<<7;
        break;
      case 'S': //Write SOS
        Option.transParam|=1<<5;
        Option.transParam|=1<<8;
        break;
      case '?'://Unknown option
        printf("  Error: %c\n", optopt);
        return 1;
      default:
        Usage(argv[0]);
        return 0;
    }
  } 
  if((~Option.transParam >> 5) & 1){
    if(strncmp(Option.file_in,_CW_FILE_NO_SET,4) == 0){  
      printf("%s: required argument and option -- '-i <filename>'\n",argv[0]);
      return 2;
    }
  }
  if(strncmp(Option.file_out,_CW_FILE_NO_SET,4) == 0){
    printf("%s: required argument and option -- '-o <filename>'\n",argv[0]);
    return 2;
  }
  if(Option.freq<=0){
    Option.freq=_CW_DEF_CARRIER; //def. val.
    //printf("Frequency auto set to %d\n",_CW_DEF_CARRIER);
  }
  if(Option.sample_rate<=0){
    Option.sample_rate=WF_SRATE; //def. val.
    //printf("Sample rate auto set to %d\n",WF_SRATE);
  }
  if(Option.wpm_speed<=0){
    Option.wpm_speed=_CW_DEF_SPEED;  //def. val.
    //printf("WPM speed auto set to %d\n",_CW_DEF_SPEED);
  }
  if(Option.wpm_speed>120){
    Option.wpm_speed=120;  //maximum 120 WPM !
    //printf("WPM speed auto set to %d\n",120);
  }
  len = TranslateToMorse(Option.freq, Option.sample_rate, Option.wpm_speed, Option.file_in, Option.file_out, Option.transParam);
  if(!silent) {
    if(strncmp(Option.file_in,_CW_FILE_NO_SET,4) == 0) {
      printf("Input file: %s, Output file: %s\n","none",Option.file_out);
    }
    else{
      printf("Input file: %s, Output file: %s\n",Option.file_in,Option.file_out);
    }
    printf("Carrier frequency %d Hz, Sample rate %d Hz\n",Option.freq,Option.sample_rate);
    printf("WPM speed %d, dot lengt %d ms\n",Option.wpm_speed,(int)(1200/Option.wpm_speed));
    printf("Total dots %d, Total time %.2lf s\n",len,1.2/Option.wpm_speed*len);
    printf("File size %d kb\n",(int)(WF_BITS/8*WF_CHAN*1.2/Option.wpm_speed*len*Option.sample_rate/1000));    
  }
  
  return 0;
}

void Usage(char *p_name) {
    printf("Morse (CW) coder from text to audio, Extended ASCII is Windows-1250!\n");
    printf("Usage: %s -MRS | -i FILE -o FILE [-w SPEED -f FREQ -r RATE -msgblWh]\n",p_name);
    printf(" Control\n");
    printf("  -w <speed>    Set morse signal speed in words per minute (WPM), default 24, 1-120\n");
    printf("  -f <freq>     Set carrier frequency of sine wave in Hz , default 1000 Hz\n");
    printf("  -r <rate>     Set audio file sampling frequency in Hz, default 16000 Hz\n");
    printf("  -i <filename> Input text file for coder\n");
    printf("  -o <filename> Output audio file in WAV format\n");
    printf(" Options\n");
    printf("  -m            Enable start/stop marks\n");
    printf("  -s            Silent mode (no text output)\n");
    printf("  -g            Message is good\n");
    printf("  -b            Message is bad\n");
    printf("  -l            Set morse speed slower\n");
    printf("  -W            Please wait\n");
    printf("  -M            Mistake\n");
    printf("  -R            Repeat message\n");
    printf("  -S            Write SOS signal\n");
    printf("  -h            Show this help\n");
    printf("                Build: %d  at %s %s, GCC %s\n", BUILDNUMBER, __TIME__, __DATE__, __VERSION__); 
}
