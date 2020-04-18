/*
 * Project Air-quality
 * Description: Air Quality Monitroing Kit  
 * Author: Talal Gedeon 
 * Date: April 16th 2020
 */

// setup() runs once, when the device is first turned on.

#define DUST_SENSOR_PIN D4
#define SENSOR_READING_INTERVAL 30000
#define AQS_PIN A2
#include <math.h>
#include "Air_Quality_Sensor.h"

AirQualitySensor aqSensor(AQS_PIN);


unsigned long lastInterval;
unsigned long lowpulseoccupancy = 0;
unsigned long last_lpo = 0;
unsigned long duration;

float ratio = 0;
float concentration = 0;

void setup() {
  Serial.begin(9600);

  pinMode( DUST_SENSOR_PIN, INPUT);
  lastInterval = millis();

  if (aqSensor.init())
 {
   Serial.println("Air Quality Sensor ready.");
 }
 else
 {
   Serial.println("Air Quality Sensor ERROR!");
 }

}

// loop() runs over and over again, as quickly as it can execute.
void loop() {
  duration = pulseIn( DUST_SENSOR_PIN, LOW);
  lowpulseoccupancy = lowpulseoccupancy + duration;

  if ((millis() - lastInterval) > SENSOR_READING_INTERVAL)
  {
    String quality = getAirQuality();
    Serial.printlnf("Air Quality: %s", quality.c_str());

    getDustSensorReadings();

    lowpulseoccupancy =0;
    lastInterval = millis();

  }
}
String getAirQuality()
{
 int quality = aqSensor.slope();
 String qual = "None";

 if (quality == AirQualitySensor::FORCE_SIGNAL)
 {
   qual = "Danger";
 }
 else if (quality == AirQualitySensor::HIGH_POLLUTION)
 {
   qual = "High Pollution";
 }
 else if (quality == AirQualitySensor::LOW_POLLUTION)
 {
   qual = "Low Pollution";
 }
 else if (quality == AirQualitySensor::FRESH_AIR)
 {
   qual = "Fresh Air";
 }

 return qual;
}

void getDustSensorReadings()
{
  // This particular dust sensor returns 0s often, so let's filter them out by making sure we only
  // capture and use non-zero LPO values for our calculations once we get a good reading.
  if (lowpulseoccupancy == 0)
  {
    lowpulseoccupancy = last_lpo;
  }
  else
  {
    last_lpo = lowpulseoccupancy;
  }

  ratio = lowpulseoccupancy / (SENSOR_READING_INTERVAL * 10.0);                   // Integer percentage 0=>100
  concentration = 1.1 * pow(ratio, 3) - 3.8 * pow(ratio, 2) + 520 * ratio + 0.62; // using spec sheet curve

  Serial.printlnf("LPO: %d", lowpulseoccupancy);
  Serial.printlnf("Ratio: %f%%", ratio);
  Serial.printlnf("Concentration: %f pcs/L", concentration);
}