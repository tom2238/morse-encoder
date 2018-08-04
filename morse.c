/* ========================================================================== */
/*                                                                            */
/*   morse.c                                                                  */
/*   (c) 2017 Tomas Dubina                                                    */
/*                                                                            */
/*   Translate txt to morse code                                              */
/*                                                                            */
/* ========================================================================== */
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include "morse.h"
#include "wavwrite.h"

unsigned short int GetChar2Bit(unsigned char alfa){
// unsigned integer 16bits
// 15 downto 12 'length' = 4bits, 11 downto 0 'data' = 12bits 
// 0111 0000 0000 0000   , 0 . , 1 - | char L (0x4C) .-.. , lengt 0x4, data 0x4, total 0x4004
//    ^ ^
// special: lengt=15 space
// Windows-1250 charset            0x0   , 0x1   , 0x2   , 0x3   , 0x4   , 0x5   , 0x6   , 0x7   , 0x8   , 0x9   , 0xA   , 0xB   , 0xC   , 0xD   , 0xE   , 0xF   ,     
unsigned short int char2bits[256]={0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x601E, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,  //0x0
                                   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,  //0x1
                                   0xF000, 0x6031, 0x6012, 0x0000, 0x7009, 0x0000, 0x5008, 0x601E, 0x5016, 0x602D, 0x0000, 0x500A, 0x6033, 0x6021, 0x6015, 0x5012,  //0x2
                                   0x501F, 0x500F, 0x5007, 0x5003, 0x5001, 0x5000, 0x5010, 0x5018, 0x501C, 0x501E, 0x6038, 0x602A, 0x0000, 0x5011, 0x0000, 0x600C,  //0x3
                                   0x601A, 0x2001, 0x4008, 0x400A, 0x3004, 0x1000, 0x4002, 0x3006, 0x4000, 0x2000, 0x4007, 0x3005, 0x4004, 0x2003, 0x2002, 0x3007,  //0x4
                                   0x4006, 0x400D, 0x3002, 0x3000, 0x1001, 0x3001, 0x4001, 0x3003, 0x4009, 0x400B, 0x400C, 0x0000, 0x0000, 0x0000, 0x0000, 0x600D,  //0x5
                                   0x0000, 0x2001, 0x4008, 0x400A, 0x3004, 0x1000, 0x4002, 0x3006, 0x4000, 0x2000, 0x4007, 0x3005, 0x4004, 0x2003, 0x2002, 0x3007,  //0x6
                                   0x4006, 0x400D, 0x3002, 0x3000, 0x1001, 0x3001, 0x4001, 0x3003, 0x4009, 0x400B, 0x400C, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,  //0x7
                                   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x3000, 0x0000, 0x3000, 0x1001, 0x400C, 0x400C,  //0x8
                                   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x3000, 0x0000, 0x3000, 0x1001, 0x400C, 0x400C,  //0x9
                                   0x0000, 0x0000, 0x0000, 0x4004, 0x0000, 0x2001, 0x0000, 0x0000, 0x0000, 0x0000, 0x3000, 0x0000, 0x0000, 0x0000, 0x0000, 0x400C,  //0xA
                                   0x0000, 0x0000, 0x0000, 0x4004, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x2001, 0x3000, 0x0000, 0x4004, 0x0000, 0x4004, 0x400C,  //0xB
                                   0x3002, 0x2001, 0x2001, 0x2001, 0x2001, 0x4004, 0x400A, 0x400A, 0x400A, 0x1000, 0x1000, 0x1000, 0x1000, 0x2000, 0x2000, 0x3004,  //0xC
                                   0x3004, 0x2002, 0x2002, 0x3007, 0x3007, 0x3007, 0x3007, 0x0000, 0x3002, 0x3001, 0x3001, 0x3001, 0x3001, 0x400B, 0x1001, 0x0000,  //0xD
                                   0x3002, 0x2001, 0x2001, 0x2001, 0x2001, 0x4004, 0x400A, 0x400A, 0x400A, 0x1000, 0x1000, 0x1000, 0x1000, 0x2000, 0x2000, 0x3004,  //0xE
                                   0x3004, 0x2002, 0x2002, 0x3007, 0x3007, 0x3007, 0x3007, 0x0000, 0x3002, 0x3001, 0x3001, 0x3001, 0x3001, 0x400B, 0x1001, 0x0000}; //0xF

   return char2bits[(unsigned short int)alfa];
   // ^^^^^^^^^^^^ It is magical? :-)  How is it stored there? :-)
}

/*void PrintCodeChar(unsigned short int value){ 
   unsigned short int lengt=0;
   unsigned short int morse=0;
   lengt = value >> 12;
   morse = value & 0x0FFF;
   if(lengt==15){
     printf("Length %d, Morse ",7);
     PrintBin(0, 7); 
   }
   else {
     printf("Length %d, Morse ",lengt);
     PrintBin(morse, lengt);
   }
   printf("\n");
} */

void WriteCodeChar(unsigned short int value, unsigned int freq, unsigned int sample_rate,unsigned int wpm, FILE *zp){
   unsigned short int lengt=0;
   unsigned short int morse=0;
   lengt = value >> 12;
   morse = value & 0x0FFF;
   if(lengt==15) { //space
     wav_space(sample_rate*1.2/wpm*7,zp);
   }
   else if(lengt==0) { //ignore this
   }
   else {
     WriteMorse(morse, lengt, freq, sample_rate, wpm, zp);
   }
}

/*void PrintBin(int x, int len) { 
   unsigned i; 
   if(len==0){len=1;}
    for (i = 1 << (len-1); i > 0; i = i / 2)  {
        (x & i)? printf("-"): printf(".");
    }
}*/

void WriteMorse(unsigned int x,unsigned int len, unsigned int freq, unsigned int sample_rate,unsigned int wpm, FILE *zp) {
   unsigned i; 
   if(len==0){len=1;}
    for (i = 1 << (len-1); i > 0; i = i / 2)  {
        if(x & i){
          WriteDash(freq, sample_rate, wpm, zp);
        }
        else{ 
          WriteDot(freq, sample_rate, wpm, 0, zp);
        }
        if(i!=1) {
          wav_space(sample_rate*1.2/wpm, zp);
        }
    }
}

double WriteDot(unsigned int freq, unsigned int sample_rate,unsigned int wpm, double phase, FILE *zp) {
   double tdur = 1.2/wpm; //dot length
   return wav_sine(freq, sample_rate*tdur, sample_rate, phase, zp);
}

void WriteDash(unsigned int freq, unsigned int sample_rate,unsigned int wpm, FILE *zp){
   int i;
   double ph=0;
   for(i=0;i<3;i++){ //dash lengt 3*dots
     ph = WriteDot(freq, sample_rate, wpm, ph, zp);
   }
}

unsigned short int GetCharLengt(unsigned short int value){
   return value >> 12;
}

int LengthChar(unsigned short int value){
   unsigned short int lengt=0;
   unsigned short int morse=0;
   lengt = value >> 12;
   morse = value & 0x0FFF;
   int result=0;	
   int i;
   if(lengt==0){
     return 0;
   }
   else if(lengt==15) { // space words
     return 7;
   } 
   else {
    for(i = 1 << (lengt-1); i > 0; i = i / 2){
     if(morse & i) {
       result+=3; // - 3 dots
	   }
     else {
       result+=1; // . 1 dot
     } 
     if(i!=1) {
       result+=1; // space bet. dot,dash
     }
    }
   }
   return result;
}

int TranslateToMorse(unsigned int freq ,unsigned int sample_rate, unsigned int wpm, char *file_in, char *file_out, unsigned int param) {
  int i,k,len=3,en_read=1;
  char msg[255];
  char line[_CW_INPUT_BUFFER];
  FILE *f_out=NULL;
  FILE *f_in=NULL;
  if((f_out = fopen(file_out, "wb")) == NULL){
    strcpy(msg, "Error opening ");
    strcat(msg, file_out);
    perror(msg);
    return -2;
  }
  if((~param >> 5) & 1){
    if((f_in = fopen(file_in, "rt")) == NULL){
      strcpy(msg, "Error opening ");
      strcat(msg, file_in);
      perror(msg);
      return -3;
    }
  }
  wav_head(5, sample_rate, WF_BITS, f_out);
  wav_space(sample_rate*1.2/wpm*3, f_out);

  if(50000>(k=(int)(1200000/wpm))){
    for(i=0;i<(int)ceil(50000/k);i++){
      wav_space(sample_rate*1.2/wpm*3, f_out);
      len+=3;
    }
  }

  if(param & 1) { //Starting signal enabled
     len+=LengthChar(_CW_START);
     WriteCodeChar(_CW_START, freq, sample_rate, wpm, f_out);
     len+=7; // space
     wav_space(sample_rate*1.2/wpm*7 , f_out);
  }
  if((param >> 1) & 1) {
     len+=LengthChar(_CW_OK); //Morse msg is OK
     WriteCodeChar(_CW_OK, freq, sample_rate, wpm, f_out);
     len+=7; // space
     wav_space(sample_rate*1.2/wpm*7 , f_out);
  }
  if((param >> 2) & 1) {
     len+=LengthChar(_CW_NOK); //Morse msg is bad
     WriteCodeChar(_CW_NOK, freq, sample_rate, wpm, f_out);
     len+=7; // space
     wav_space(sample_rate*1.2/wpm*7 , f_out);
  }
  if((param >> 3) & 1) {
     len+=LengthChar(_CW_SLOW); //Set morse speed slower
     WriteCodeChar(_CW_SLOW, freq, sample_rate, wpm, f_out);
     len+=7; // space
     wav_space(sample_rate*1.2/wpm*7 , f_out);
  }
  if((param >> 4) & 1) {
     len+=LengthChar(_CW_WAIT); //Please wait
     WriteCodeChar(_CW_WAIT, freq, sample_rate, wpm, f_out);
     len+=7; // space
     wav_space(sample_rate*1.2/wpm*7 , f_out);
  }  
  if(((param >> 5) & 1) && ((param >> 6) & 1)) { //Mistake
     len+=WriteMorseWord(freq ,sample_rate, wpm, _CW_W_FAIL , f_out);
     en_read = 0;
  }
  if(((param >> 5) & 1) && ((param >> 7) & 1)) { //Repeat message
     len+=WriteMorseWord(freq ,sample_rate, wpm, _CW_W_REPEAT , f_out);
     en_read = 0;
  }
  if(((param >> 5) & 1) && ((param >> 8) & 1)) { //Send SOS signal
     len+=WriteMorseWord(freq ,sample_rate, wpm, _CW_W_SOS , f_out);
     en_read = 0;
  }
  while (en_read) {
    if (fgets(line,_CW_INPUT_BUFFER, f_in)=='\0') break; // line per line
      i=0;
      while(line[i]!='\0'){
        if(GetCharLengt(GetChar2Bit(line[i]))==0){ //skip not know characters
          i++;
          continue;
        }
        len+=LengthChar(GetChar2Bit(line[i]));
        WriteCodeChar(GetChar2Bit(line[i]), freq, sample_rate, wpm, f_out);
    
        if(line[i]==' '){
          //Space
        }
        else if(line[i+1]==' ' && line[i]!=' '){
          //Space after word 
        }
        else {
          wav_space(sample_rate*1.2/wpm*3, f_out);
          len+=3; //here**
        }
        i++;
      } 
      len+=4; //new line space 3+4 here**
      wav_space(sample_rate*1.2/wpm*4 , f_out);
  } 
  if(param & 1) { // Ending signal enabled
     len+=LengthChar(_CW_END); 
     WriteCodeChar(_CW_END, freq, sample_rate, wpm, f_out);
     len+=7; // space
     wav_space(sample_rate*1.2/wpm*7 , f_out);
  }
  double WavTime = 1.2/wpm*len;

  fseek(f_out, 0, SEEK_SET);  //move to start, file
  wav_head(WavTime, sample_rate, WF_BITS, f_out); //rewrite header
  
  fclose(f_out);
  if((~param >> 5) & 1){
    fclose(f_in);
  }
  
  return len;
}

int WriteMorseWord(unsigned int freq ,unsigned int sample_rate, unsigned int wpm, char *text, FILE *zp) {
  int i=0;
  int len=0;
  while(text[i]!='\0'){
   if(GetCharLengt(GetChar2Bit(text[i]))==0){ //skip not know characters
     i++;
     continue;
   }
   len+=LengthChar(GetChar2Bit(text[i]));
   WriteCodeChar(GetChar2Bit(text[i]), freq, sample_rate, wpm, zp);
   if(text[i+1]!=' ') {
     wav_space(sample_rate*1.2/wpm*3, zp);
     len+=3; //here**
   }
   i++;
  }
  len+=4; // space 3+4 here**
  wav_space(sample_rate*1.2/wpm*4 , zp);
  return len; 
}
