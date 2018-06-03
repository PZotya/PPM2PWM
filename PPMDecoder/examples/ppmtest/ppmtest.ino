#include "PPMDecoder.h"
void setup() {
  PPM.begin(2); //The PPM signal is received in pin 2
  PPM.setChannnel(1,PPM2PWM,4); // first channel goes as a PWM output to pin 4
  PPM.setChannnel(2,PPM2PWM,5); // next channel goes as a PWM output to pin 5
  PPM.setChannnel(3,PPM2PWM,6); // next channel goes as a PWM output to pin 6
  PPM.setChannnel(4,PPM2PWM,7); // next channel goes as a PWM output to pin 7
  PPM.setChannnel(5,PPMSelector,8); // the channe 5 is for toggle the HIGH state between pin 8 and 9
  Serial.begin(9600);
}

void loop() {
   for( int i=1;i<6;i++){ // let's see the first 6 channel
    Serial.print("Channel(");
    Serial.print(i);
    Serial.print("):");
    Serial.println(PPM.getChannelValue(i)); // the value in microsec
   }
   delay(1000);
}
