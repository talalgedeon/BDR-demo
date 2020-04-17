/******************************************************/
//       THIS IS A GENERATED FILE - DO NOT EDIT       //
/******************************************************/

#include "Particle.h"
#line 1 "/Users/talalagedeon/Desktop/particle-coding/Air-quality/src/Air-quality.ino"
/*
 * Project Air-quality
 * Description: Air Quality Monitroing Kit  
 * Author: Talal Gedeon 
 * Date: April 16th 2020
 */

// setup() runs once, when the device is first turned on.

void setup();
void loop();
#line 10 "/Users/talalagedeon/Desktop/particle-coding/Air-quality/src/Air-quality.ino"
#define DUST_SENSOR_PIN D4
#define SENSOR_READING_INTERVAL 30000
#include <math.h>


unsigned long lastInterval;
unsigned long lowpulseoccupancy = 0;
unsigned long last_lpo = 0;
unsigned long duration;

float ratio = 0;
float concentration = 0;
void getDustSensorReadings();

void setup() {
  Serial.begin(9600);

  pinMode( DUST_SENSOR_PIN, INPUT);
  lastInterval = millis();

}

// loop() runs over and over again, as quickly as it can execute.
void loop() {
  duration = pulseIn( DUST_SENSOR_PIN, LOW);
  lowpulseoccupancy = lowpulseoccupancy + duration;

  if ((millis() - lastInterval) > SENSOR_READING_INTERVAL)
  {

    getDustSensorReadings();

    lowpulseoccupancy =0;
    lastInterval = millis();
  }

  if (lowpulseoccupancy == 0)
  {
    lowpulseoccupancy = last_lpo;
  }
  else
  {
    last_lpo= lowpulseoccupancy;
  }
  
  ratio = lowpulseoccupancy / (SENSOR_READING_INTERVAL * 10.0);
  concentration = 1.1 * pow(ratio, 3) - 3.8 * pow(ratio, 2) + 520 * ratio + 0.62;

  Serial.printlnf("LPO: %d", lowpulseoccupancy);
  Serial.printlnf("Ratio: %f%%", ratio);
  Serial.printlnf("Concentration: %f pcs/L", concentration);


}