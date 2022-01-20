#include <ADCTouch.h>
#include <SPI.h>

#define on LOW
#define off HIGH
#define SAMPLES 50
#define CALIBRATE_SAMPLES 100
#define DELAYTIME 0

#define but1 A0
#define but2 A2
#define but3 A1
#define but4 A5
#define but5 A3
#define but6 A4

#define but1LED 8
#define but2LED 7
#define but3LED 6
#define but4LED 4
#define but5LED 3
#define but6LED 2

#define calibrateSignalPin 9

int buts[6] = {but1, but2, but3, but4, but5, but6};
int refs[6] = {0, 0, 0, 0, 0, 0};
int values[6] = {0, 0, 0, 0, 0, 0};
int thresholds[6] = {5, 11, 10, 5, 5, 3};
int leds[6] = {but1LED, but2LED, but3LED, but4LED, but5LED, but6LED};

char sendValues[6] = {'1', '2', '3', '4', '5', '6'};

int counter = 0;
int oneTimeCounter = 0;
int calibrateFlag = 0;

void setup() {
  
  Serial.begin(9600);
  SPI.begin();

  for(int i = 0; i<6; i++)
  {
    pinMode(leds[i], OUTPUT);
    digitalWrite(leds[i], off);
  }
  pinMode(calibrateSignalPin, INPUT);
  
  //calibrate();
  delay(1000);

}

void loop() {

  if(counter == 1000 || oneTimeCounter == 10)
  {
    getRefs();
    counter = 0;
  }
  
  calibrateFlag = digitalRead(calibrateSignalPin);
  if(calibrateFlag == 1)
  {
    calibrate();
  }
 
  for(int i = 0; i<6; i++)
  {
    values[i] = (ADCTouch.read(buts[i], SAMPLES) - refs[i]); 
    Serial.print(values[i]);
    Serial.print("   ");
    if(values[i] > thresholds[i])
    {
      digitalWrite(leds[i], on);
      SPI.transfer(sendValues[i]);
    }
    else
    {
      digitalWrite(leds[i], off);
    }
    delay(DELAYTIME);
  }
  
  Serial.println("");
  counter++;
  oneTimeCounter++;
}

void getRefs()
{
 delay(100);
 for(int i = 0; i<6; i++)
 {
   refs[i] = ADCTouch.read(buts[i], 200);
   delay(DELAYTIME);
 }
 delay(100);
}

void calibrate()
{
  for(int i = 0; i<6; i++)
  {
    digitalWrite(leds[i], on);
  }
  delay(2000);
  
  for(int i = 0; i<6; i++)
  {
    digitalWrite(leds[i], off);
  }
  delay(1000);
  
  for(int i = 0; i<6; i++)
  {
    digitalWrite(leds[i], on);
    delay(2000);
    getRefs();
    digitalWrite(leds[i], off);
    delay(2000);
  
    for(int j = 0; j<CALIBRATE_SAMPLES; j++)
    {
      values[i] += (ADCTouch.read(buts[i], SAMPLES) - refs[i]);
    }
    values[i] = values[i]/CALIBRATE_SAMPLES;
    delay(500);
  }
  thresholds[0] = values[0] - 2;
  thresholds[1] = values[1] - 3;
  thresholds[2] = values[2] - 3;
  thresholds[3] = values[3] - 3;
  thresholds[4] = values[4] - 8;
  thresholds[5] = values[5] - 8;

  Serial.println("Threshold Values");
  for(int i = 0; i<6; i++)
  {
    Serial.print(thresholds[i]);
    Serial.print("   ");
  }
  Serial.println("");
  
  for(int i = 0; i<6; i++)
  {
    digitalWrite(leds[i], on);
  }
  delay(2000);
  
  for(int i = 0; i<6; i++)
  {
    digitalWrite(leds[i], off);
  }
  delay(1000);
  getRefs();
  delay(1000);
}
