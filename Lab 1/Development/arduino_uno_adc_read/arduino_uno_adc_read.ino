#include "avdweb_AnalogReadFast.h"
/*
   Read adc as fast as possible using all the tricks.
*/
int const totalSamples = 1536;
byte buffer[totalSamples];

// Define various ADC prescaler
const unsigned char PS_16 = (1 << ADPS2);
const unsigned char PS_32 = (1 << ADPS2) | (1 << ADPS0);
const unsigned char PS_64 = (1 << ADPS2) | (1 << ADPS1);
const unsigned char PS_128 = (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);


void setup() {
  Serial.begin(115200);
  pinMode(A0, INPUT);


  // set up the ADC
  ADCSRA &= ~PS_128;  // remove bits set by Arduino library

  // you can choose a prescaler from above.
  // PS_16, PS_32, PS_64 or PS_128
  ADCSRA |= PS_32;


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
  unsigned long start_times = micros();
  for (int i = 0; i < totalSamples; i ++) {
    buffer[i] = analogReadFast(A0) >> 2;
  }
  unsigned long stop_times = micros();
  
  // /*
    for (int i = 0; i < totalSamples; i ++) {
      Serial.print(buffer[i]);
      Serial.write(',');
    }
  //  */
  
  Serial.println(stop_times - start_times);
  Serial.flush();
}
