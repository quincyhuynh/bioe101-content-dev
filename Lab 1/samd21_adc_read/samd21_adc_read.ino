#include "avdweb_AnalogReadFast.h"
/*
   Read adc as fast as possible using all the tricks.
*/
int const adc_pin = A0;
int const totalSamples = 1024*2;
int buffer[totalSamples];


void setup() {
  Serial.begin(115200);
  pinMode(adc_pin, INPUT);
  analogReadResolution(12);
  delay(100);
}


void loop() {
  if (Serial.available()) {
    if (Serial.available() > 0) {
      char cmd = Serial.read();  //gets one byte from serial buffer
      if (cmd == 's') {
        sample_and_send();
      }
    }
  }
  delay(10);
}

void sample_and_send() {
  unsigned long  start_times = micros();
  for (int i = 0; i < totalSamples; i ++) {
    buffer[i] = analogReadFast(adc_pin);
    delayMicroseconds(50);
  }
  unsigned long  stop_times = micros();
  
  // /*
    for (int i = 0; i < totalSamples; i ++) {
      Serial.print(buffer[i]);
      Serial.write(',');
    }
  //  */
  
  Serial.println((stop_times - start_times));
  Serial.flush();
}
