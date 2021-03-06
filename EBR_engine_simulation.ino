//This is an engine sensor simulator designed for the EBR 1190rx/sx to diagnose a failed ECM.
//It is designed for a Teensy3.5 board https://www.pjrc.com/store/teensy35.html
//It's suggested that all testing be done with the ECM out of the bike, or disconnect all
//additional sensors, injectors and relays. The Author of this code assumes no responsibility
//or liabilty for any damage to product during the execution of this code.
long previousMicros = 0;
long speedMicros = 0;
long int speedRestart;
long int speedTime; //A variable used to store the time to execute one complete wave cycle at the given frequency
float inputFreq; //The value input from the serial monitor
float speedFreq; //A variable to store the frequency for simulating the speed sensor tone ring
float rpmFreq; //A variable used to store the frequency for simulating the crankshaft sensor output
float rpmTime; //This is the variable used to store the period used to execute 34 wave cycles at the given frequency
float rpmRestart; //This variable stores the period used to execute 36 wave cycles at the given frequency
unsigned long currentMicros = 0; 

//This function takes the serial monitor input, applies it to be used as the simulated crank shaft sensor frequency,
//then calculates the period of the waveform in microseconds. We then calculate
//the microseconds needed to generate 34 wave forms, and 36 wave forms. This is used to properly simulate
//the step pattern of the crank shaft(34 steps, then a blank spot the equivalent to 2 steps.)
void rpmFunction() {
  //Calculate the time period to execute 34 wave cycles at the freequency input from the serial monitor
  float rpmTime = 1 / rpmFreq * 1000000 * 34;
  //Calculate the time period to execute 36 wave cycles at the frequency input from the serial monitor
  float rpmRestart = 1 / rpmFreq * 1000000 * 36;
  //
  if (currentMicros - previousMicros < rpmTime) {
    //Generate a tone on pin 5 at the frequency input from the serial monitor
    tone(5, rpmFreq);
  }
  if (currentMicros - previousMicros > rpmTime) {
    //stop tone on pin 5
    noTone(5);
  }
  if (currentMicros - previousMicros > rpmRestart) {
    previousMicros = currentMicros;
  }
}
//This function takes the serial monitor input, applies it to be used to simulate the speed sensor frequency,
//it then, calculates the period of the waveform in microseconds,and genarates a square wave, by
//rapidly switching a pin between high and low.
void speedFunction() {
  float speedTime = 1 / speedFreq * 1000000/2;
  float speedRestart = speedTime * 2;
  if (currentMicros - speedMicros < speedTime) {
    digitalWrite(35, HIGH);
  }
  if (currentMicros - speedMicros > speedTime) {
    digitalWrite(35, LOW);
  }
  if (currentMicros - speedMicros > speedRestart) {
    speedMicros = currentMicros;
  }
}

void setup() {
  //set pin35 to output. This will be the pin used to simulate the speed sensor
  pinMode(35, OUTPUT);
}

void loop() {
  //start counting microseconds.
  currentMicros = micros();

  if (Serial.available()) {
    inputFreq = Serial.parseInt();
  }
  // If the serial input value is greater than 1000, assign the value as an RPM value,
  // and continue generating speed waveform at current frequency
  if (inputFreq > 1000) {
    rpmFreq = inputFreq;
    rpmFunction();
    speedFunction();

  }

  // If the serial input value is less than 1000, assign the value as a speed value,
  // and continue generating RPM waveform at current frequency
  if (inputFreq < 1000) {
    speedFreq = inputFreq;
    speedFunction();
    rpmFunction();
  }
}
