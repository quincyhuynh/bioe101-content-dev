const int sig = A0;
float interval = 1000.0; //us
// [1 / interval (us)] * [10^6 us / sec] = samples per sec
float sig_val = 0;
int i = 0;

// Define various ADC prescaler
// defines for setting and clearing register bits
#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif
#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif

// set prescale to 16
sbi(ADCSRA,ADPS2) ;
cbi(ADCSRA,ADPS1) ;
cbi(ADCSRA,ADPS0) ;

void setup() {
  // set pin for ekg to input
  pinMode(sig, INPUT);
  // high baud rate
  Serial.begin(115200);
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
