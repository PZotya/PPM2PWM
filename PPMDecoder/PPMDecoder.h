/*
  PPMDecoder.h - Arduino PPM signal processing library
  Written by Zoltan Palko
  Released into the public domain.

  Uses interrupt for signal receiving.
  */ 

#include "Arduino.h"
#ifndef PPMDecoder_h
#define PPMDecoder_h

#define PPMMAXCHANNELNUM 8 // the maximum channel number in the PPM stream. Do not set it higher than the phisical channel number
#define PPMNONE 0			// do nothing mode
#define PPM2PWM 1			// the PPM - PWM transfer mode
#define PPM2Switch 2		// the output is switced by the PPM value. The on is above the medium value 
#define PPMSwitchInverted 3	// the output is switced by the PPM value. The on is beneath the medium value 
#define PPMSelector 4		// this mode uses two output, the first is on is beneath the medium value, the second is inverted. The outputs must follow each other!

class PPMDecoder
{
  public:
	void begin(int inputPort);	//initialization of PPM decoder. The input is the port of the signal input
    void setChannnel(int channel, int mode, int outPort); // the setup of each channel. The channel number starts from 1
	int getChannelValue(int channel); // get the recent channel value
	void processing();     //for connectivity between the interrupt and the class, do not use it
	
  private:
     struct chInfo{
		 byte mode;
		 int value;
		 byte port;
	 };
	volatile unsigned long _lastTime; // the signal rising time
	volatile int _chCount;		// the actual cannel number. Starts from 0
	volatile boolean _inReceive; // wether in receivin state

	volatile chInfo _channelList[PPMMAXCHANNELNUM]; // data storage for PPM channels 

};

extern PPMDecoder PPM; // implementation for class declaration 
#endif
