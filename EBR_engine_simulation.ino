//This is an engine sensor simulator designed for the EBR 1190rx/sx to diagnose a failed ECM.
//It is designed for a Teensy3.5 board https://www.pjrc.com/store/teensy35.html
//It's suggested that all testing be done with the ECM out of the bike, or disconnect all
//additional sensors, injectors and relays. The Author of this code assumes no responsibility
//or liabilty for any damage to product during the execution of this code.

#define Num_Samples  49
#define Num_Waves  33
#define MaxWaveTypes 4
int i = 0;
int wavecnt = 0;


// Wave table used to generate a wave form that simulates the crank shaft sensor.
static byte WaveFormTable[MaxWaveTypes][Num_Samples] = {
   // Sin wave
   {
0x82, 0x70, 0x5F, 0x4F, 0x41, 0x34, 0x29, 0x21, 0x1C, 0x19, 0x1A, 0x1D, 0x24, 0x2D, 0x38, 0x46, 
0x55, 0x66, 0x77, 0x88, 0x99, 0xAA, 0xB9, 0xC7, 0xD2, 0xDB, 0xE2, 0xE5, 0xE6, 0xE3, 0xDE, 0xD6, 
0xCB, 0xBE, 0xB0, 0xA0, 0x8F 
   },

   // empty
   { 
    0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 
    0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 
    0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 
    0x80
   },
   // Sin wave inverted
   {
    0x80, 0x8D, 0x99, 0xA6, 0xB2, 0xBD, 0xC7, 0xD0, 0xD7, 0xDD, 0xE2, 0xE5, 0xE6, 0xE5, 0xE3, 0xE0,
    0xDA, 0xD3, 0xCB, 0xC2, 0xB7, 0xAC, 0xA0, 0x93, 0x86, 0x79, 0x6C, 0x5F, 0x53, 0x48, 0x3D, 0x34, 
    0x2C, 0x25, 0x1F, 0x1C, 0x1A, 0x19, 0x1A, 0x1D, 0x22, 0x28, 0x2F, 0x38, 0x42, 0x4D, 0x59, 0x66, 
    0x72
   }
};

void setup() {

}

void loop() {
  //basic square wave generated to simulate speed sensor. First value represents pin the wave is output on
  //second represents the frequency of the wave. 200hz≈75mph.
  //wire pin 22 to the white wire of the vehicle speed sensor input.
  tone(22, 200);

  //assign wavetables to variables
  byte wave_type = 0; // Sine
  byte no_wave = 1; // no wave
  byte wave_type2 = 2; // inverted Sine

  //generate wave form. 24 wave cycles, then blank spot equivalent to 2 wave cycles.
  //output on A21 and A22. Wire A21 to red/beige on crank shaft sensor input 
  //and A22 to black/beige on the crank shaft sensor input. It should simulate about 5430 rpm
  if (wavecnt <= Num_Waves){ 
  analogWrite(A21, WaveFormTable[wave_type][i]);
  analogWrite(A22, WaveFormTable[wave_type2][i]);
  }else if (wavecnt > Num_Waves && wavecnt <= 35) {
    analogWrite(A21, WaveFormTable[no_wave][i]);
    analogWrite(A22, WaveFormTable[no_wave][i]); 
  } else{
    wavecnt = 0;
  }
  i++;
  if (i >= Num_Samples) {
    i = 0;
    wavecnt++;
    Serial.print(wavecnt); 
}
}
