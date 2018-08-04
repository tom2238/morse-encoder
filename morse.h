/* ========================================================================== */
/*                                                                            */
/*   morse.h                                                                  */
/*   (c) 2017 Tomas Dubina                                                    */
/*                                                                            */
/*   Translate txt to morse code                                              */
/*                                                                            */
/* ========================================================================== */
#ifndef _TXT_TO_MORSE
#define _TXT_TO_MORSE
//No character signals
#define _CW_START 0x80AA
#define _CW_END 0x6005
#define _CW_OK 0x603D
#define _CW_NOK 0x7000
#define _CW_SLOW 0x7054
#define _CW_WAIT 0x5008
//Message
#define _CW_W_FAIL "eeeeee"
#define _CW_W_REPEAT "iiiiii"
#define _CW_W_SOS "sos"
//Buffer length
#define _CW_INPUT_BUFFER 1024
//Default values
#define _CW_DEF_SPEED 24
#define _CW_DEF_CARRIER 1000

//Morse code Look up table function
unsigned short int GetChar2Bit(unsigned char alfa);
//print character
/*void PrintCodeChar(unsigned short int value);*/
//Printf binary data
/*void PrintBin(int x, int len);*/
// Length of char in number of dots
int LengthChar(unsigned short int value);
//write one dot, return phase
double WriteDot(unsigned int freq, unsigned int sample_rate,unsigned int wpm, double phase, FILE *zp);
//write one dash
void WriteDash(unsigned int freq, unsigned int sample_rate,unsigned int wpm, FILE *zp);
//write one morse character
void WriteMorse(unsigned int x,unsigned int len, unsigned int freq, unsigned int sample_rate,unsigned int wpm, FILE *zp);
//decode to length and morse char
void WriteCodeChar(unsigned short int value, unsigned int freq, unsigned int sample_rate, unsigned int wpm, FILE *zp);
//general func, param = special parameters (message,no char. signals)
//PARAM bits from LSB, set 1 to enable func 
// 0 Start/Stop signal
// 1 Morse message is OK
// 2 Morse message is bad
// 3 Set morse speed slower
// 4 Please wait
// 5 Dont send file content, works with 6,7 and 8 bit
// 6 Mistake
// 7 Repeat message
// 8 SOS signal
int TranslateToMorse(unsigned int freq ,unsigned int sample_rate, unsigned int wpm, char *file_in, char *file_out, unsigned int param);
//return char len in dots
unsigned short int GetCharLengt(unsigned short int value);
//Write one word, without spaces
int WriteMorseWord(unsigned int freq ,unsigned int sample_rate, unsigned int wpm, char *text, FILE *zp);
#endif
