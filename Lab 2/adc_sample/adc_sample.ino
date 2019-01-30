#include "avdweb_AnalogReadFast.h"
/*
   Read adc as fast as possible using all the tricks.
*/
int const ADC_PIN = A0;
int const DATA_SIZE = 1024;
int16_t buffer[DATA_SIZE];


void setup() {
  Serial.begin(2000000);
  pinMode(ADC_PIN, INPUT);
  analogReadResolution(12);
  delay(100);
}


void loop() {
  if (Serial.available()) {
    if (Serial.available() > 0) {
      char cmd = Serial.read();  //gets one byte from serial buffer
      if (cmd == 's') {
        ADC_SAMPLE();
      }
    }
  }
  delay(10);
}

void ADC_SAMPLE() {
  unsigned long start_sample = micros();
  for (int i = 0; i < DATA_SIZE; i ++) {
    buffer[i] = analogReadFast(ADC_PIN);
    delayMicroseconds(50);
  }
  unsigned long stop_sample = micros();
  
  // /*
    for (int i = 0; i < DATA_SIZE; i ++) {
      Serial.print(buffer[i]);
      Serial.write(',');
    }
  //  */
  int FS = 1e6*DATA_SIZE/(stop_sample - start_sample);
  Serial.print(FS);
  Serial.write(',');
  Serial.println(DATA_SIZE);
  Serial.flush();
}
