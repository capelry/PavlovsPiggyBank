#include <PWM.h>
#include "FastLED.h"

#define ROTARY_ANGLE_SENSOR A0 // name the anolog pin being used for input
#define SPEAKER_PIN 3
#define VCC 5 // VCC value used (5V arduino pin)
#define FULL_ANGLE 300

CRGB leds[9];

float voltage;
int sensorValue;
float angle;
float freq;

// notes
int notes[4] = {260, 330, 392, 523};
// leds values
int r[4] = {50, 0, 25, 0};
int g[4] = {0, 0, 25, 50};
int b[4] = {0, 50, 0, 0};

bool canActivate;



void setup() {

  InitTimersSafe();
  Serial.begin(9600);
  pinMode(ROTARY_ANGLE_SENSOR, INPUT);
  pinMode(SPEAKER_PIN, OUTPUT);
  Serial.println("start");
  SetPinFrequencySafe(SPEAKER_PIN, 256);
  analogWrite(SPEAKER_PIN, 0);
  FastLED.addLeds<NEOPIXEL, 7>(leds, 9);
  
}

void loop() {
  
  // use this value to prevent multiple activations from one coin
  canActivate = true;

  while (true)
  {
    sensorValue = analogRead(ROTARY_ANGLE_SENSOR);
    voltage = (float)sensorValue * VCC / 1023; //1024 per volt?
    angle = voltage * FULL_ANGLE / VCC;
    Serial.println("Angle: ");
    Serial.println(angle);
  
    if (angle <= 30)
    {
      canActivate = true;
    }
    if (angle > 30 && canActivate)
    {
      playMelody();
      canActivate = false;
    }
  }
  

  
  /*
  delay(2000);
  
  analogWrite(SPEAKER_PIN, 50);
  playMelody();
  */
}

void playMelody() {
  
  analogWrite(SPEAKER_PIN, 50);
  for (int i=0; i<4; i++)
  {
    for (int j=0; j<9; j++)
    {
      leds[j].setRGB(r[i], g[i], b[i]);
    }
    FastLED.show();
    
    freq = notes[i];
    SetPinFrequencySafe(SPEAKER_PIN, freq);
    delay(500);
  }
  analogWrite(SPEAKER_PIN, 0);
  for (int j=0; j<9; j++)
    {
      leds[j].setRGB(0, 0, 0);
    }
  FastLED.show();
}
