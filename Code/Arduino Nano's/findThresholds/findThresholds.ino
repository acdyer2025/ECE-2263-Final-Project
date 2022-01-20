#include <SPI.h>


char send1 = '0';
char send2 = '*';
char send3 = '#';

void setup()
{
  Serial.begin(9600);
  SPI.begin();
  pinMode(9, INPUT);
  pinMode(13, OUTPUT);
}
void loop()
{
  int test = digitalRead(9);
  Serial.println(test);

  if(test == 1)
  {
    digitalWrite(13, HIGH);
  }
  else
  {
    digitalWrite(13, LOW);
  }
}
