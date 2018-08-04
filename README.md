Morse 1.0
=========

Encodes a text file into morse and saves as a WAV file. School project.

Information about morse code:
* http://morseovaabeceda.cz/tabulka/
* http://sv8gxc.blogspot.com/2010/09/morse-code-101-in-wpm-bw-snr.html

```
Morse (CW) coder from text to audio, Extended ASCII is Windows-1250!`
Usage: ./morse -MRS | -i FILE -o FILE [-w SPEED -f FREQ -r RATE -msgblWh]`
 Control
  -w <speed>    Set morse signal speed in words per minute (WPM), default 24, 1-120
  -f <freq>     Set carrier frequency of sine wave in Hz , default 1000 Hz
  -r <rate>     Set audio file sampling frequency in Hz, default 16000 Hz
  -i <filename> Input text file for coder
  -o <filename> Output audio file in WAV format
 Options`
  -m           Enable start/stop marks
  -s            Silent mode (no text output)
  -g            Message is good
  -b            Message is bad
  -l            Set morse speed slower
  -W            Please wait
  -M            Mistake
  -R            Repeat message
  -S            Write SOS signal
  -h            Show this help
                Build: 249  at 13:16:14 Jan  7 2018, GCC 5.3.0
```
Makefile:
* build - build program
* clean - clean compiled o files and binary
* dep - create file list
* distrib - create tar.gz package with source code
* packall - pack all files to tar.gz
