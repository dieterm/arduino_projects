#include <Wire.h>
#include <Adafruit_ADS1X15.h>

//Adafruit_ADS1015 ads1015;
//Adafruit_ADS1115 ads;

void setup(void)
{
  //ads.setGain(GAIN_FOUR);
  //ads.begin();
  
  Serial.begin(9600);
 //Serial.println("ADC0 ADC1 ADC2 ADC3 A0");
  Serial.println("A0");
  
}

void loop(void)
{
  /*int16_t adc0, adc1, adc2, adc3;

  adc0 = ads.readADC_SingleEnded(0);
  adc1 = ads.readADC_SingleEnded(1);
  adc2 = ads.readADC_SingleEnded(2);
  adc3 = ads.readADC_SingleEnded(3);*/
  int sensorValue = analogRead(A0);
  
  /*Serial.print(adc0);
  Serial.print("\t"); // or Serial.print(" ")
  
  Serial.print(adc1);
  Serial.print("\t"); // or Serial.print(" ")
  
  Serial.print(adc2);
  Serial.print("\t"); // or Serial.print(" ")

  Serial.print(adc3);
  Serial.print("\t"); // or Serial.print(" ")
  */
  Serial.println(sensorValue);
  //Serial.println("\t"); // or Serial.print(" ")
  
  
 // delay(10);
}
