const int sig = A0;
float interval = 1000.0; //us
// [1 / interval (us)] * [10^6 us / sec] = samples per sec
float sig_val = 0;
int i = 0;

// Define various ADC prescaler
const unsigned char PS_16 = (1 << ADPS2);
const unsigned char PS_32 = (1 << ADPS2) | (1 << ADPS0);
const unsigned char PS_64 = (1 << ADPS2) | (1 << ADPS1);
const unsigned char PS_128 = (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);

void setup() {
  // set pin for ekg to input
  pinMode(sig, INPUT);
  // high baud rate
  Serial.begin(115200);

  // set up the ADC
  ADCSRA &= ~PS_128;  // remove bits set by Arduino library

  // you can choose a prescaler from above.
  // PS_16, PS_32, PS_64 or PS_128
  ADCSRA |= PS_32;    // set our own prescaler to 64 
}

float curr_micros = 0.0;
float total_micros = 0.0;
float previous_micros = 0.0; 

void loop() {
  curr_micros = micros();
  if(curr_micros - previous_micros > interval) {
    // save the last time sampled 
    previous_micros = curr_micros;
    sig_val = 5.0*analogRead(sig)/1024.0;
    Serial.println(sig_val);
  }
}
