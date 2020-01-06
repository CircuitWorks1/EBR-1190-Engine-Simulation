
//This is an engine sensor simulator designed for the EBR 1190rx/sx to diagnose a failed ECM.
//It is designed for a Teensy3.5 board https://www.pjrc.com/store/teensy35.html
//It's suggested that all testing be done with the ECM out of the bike, or disconnect all
//additional sensors, injectors and relays. The Author of this code assumes no responsibility
//or liabilty for any damage to product during the execution of this code.
long previousMicros = 0;
long speedMicros = 0;
long int speedRestart;
long int speedTime;
float inputFreq;
float speedFreq;
float rpmFreq;
float rpmTime;
float rpmRestart;
unsigned long currentMicros = 0;

int i = 0;
int x = 0;
String rpmControl;

//This function takes the serial monitor input, applies it to be used as the simulated crank shaft sensor frequency,
//then calculates the period of the waveform in microseconds. We then calculate
//the microseconds needed to generate 34 wave forms, and 36 wave forms. This is used to properly simulate
//the step pattern of the crank shaft(34 steps, then a blank spot the equivalent to 2 steps.)
void rpmFunction() {
  float rpmTime = 1 / rpmFreq * 1000000 * 34;
  float rpmRestart = 1 / rpmFreq * 1000000 * 36;
  if (currentMicros - previousMicros < rpmTime) {
    tone(5, rpmFreq);
  }
  if (currentMicros - previousMicros > rpmTime) {
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
  float speedTime = 1 / speedFreq * 1000000;
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
  pinMode(35, OUTPUT);
}

void loop() {
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
