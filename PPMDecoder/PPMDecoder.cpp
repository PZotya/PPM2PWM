/*
  PPMDecoder.cpp - Arduino PPM signal processing library
  Written by Zoltan Palko
  Released into the public domain.

  Uses interrupt for signal receiving.
  */ 


#include "Arduino.h"
#include "PPMDecoder.h"
//#include "avr/interrupt.h"


static void ISR_PPM_Receiver(void) //incoming signal receiver interrupt service routine
{
	noInterrupts();
  PPM.processing();
}

PPMDecoder PPM;




void PPMDecoder::begin(int inputPort) // set upt the interrrupt to get receive
{

	pinMode(inputPort,INPUT);
	attachInterrupt(digitalPinToInterrupt(inputPort),ISR_PPM_Receiver , RISING );
	for ( int i=0;i<PPMMAXCHANNELNUM;i++){
		_channelList[i].mode=0;
		_channelList[i].value=0;
	}
	_inReceive=false;
	_lastTime=0;
}

void PPMDecoder::processing()	// te receiver rutine
{
	unsigned long currenttime;
	
	currenttime=micros();
	if (_inReceive){
		if (currenttime>_lastTime)		//avoid overrun
		_channelList[_chCount].value=currenttime-_lastTime; // the time setting
		switch(_channelList[_chCount].mode){	// what to do with the channel
			case PPM2PWM:	// close the PWM pulse
				digitalWrite(_channelList[_chCount].port,LOW);
				break;
			case PPM2Switch: //set a switch output
				if (_channelList[_chCount].value>1500) {
					digitalWrite(_channelList[_chCount].port,HIGH);
				} else {
					digitalWrite(_channelList[_chCount].port,LOW);
				}
				break;
			case PPMSwitchInverted: //set a switch output - inverted way
				if (_channelList[_chCount].value<1500) {
					digitalWrite(_channelList[_chCount].port,HIGH);
				} else {
					digitalWrite(_channelList[_chCount].port,LOW);
				}
				break;
			case PPMSelector:	// toggle the two outputs
				if (_channelList[_chCount].value>1500) {
					digitalWrite(_channelList[_chCount].port+1,HIGH);
					digitalWrite(_channelList[_chCount].port,LOW);
				} else {
					digitalWrite(_channelList[_chCount].port,HIGH);
					digitalWrite(_channelList[_chCount].port+1,LOW);
				}
				break;
		}
		_chCount++;
		if (_chCount==PPMMAXCHANNELNUM){ // end of stream
			_chCount=0;
			_inReceive=false;
		} else {
			switch(_channelList[_chCount].mode){	// next channel counting starts
				case PPM2PWM:
					digitalWrite(_channelList[_chCount].port,HIGH);
					break;
			}
		}
	} else {
		if ((currenttime-_lastTime>3000)&& (_lastTime!=0)){ // the first signal after the sync time
			_chCount=0;
			_inReceive=true;
			switch(_channelList[_chCount].mode){
				case PPM2PWM:
				digitalWrite(_channelList[_chCount].port,HIGH);
				break;
			}
		}
	}
_lastTime=currenttime;
}

void PPMDecoder::setChannnel(int channel, int mode, int outPort)
{
 	_channelList[channel-1].mode=mode;
	_channelList[channel-1].port=outPort;
	if (mode>PPMNONE){
		pinMode(outPort,OUTPUT);
		digitalWrite(outPort,LOW );
		if (mode==PPMSelector){
			pinMode(outPort+1,OUTPUT);
			digitalWrite(outPort+1,LOW );
		}
	}
}



int PPMDecoder::getChannelValue(int channel)
{
	return(_channelList[channel-1].value);
}

