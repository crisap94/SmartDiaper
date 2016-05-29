#include <Arduino.h>
#include <RainSensor.h>

// #define DEBUGGIN
/**
 * Constructor
 * param analogPin The analog Pin of the Arduino
 * param interval Is the time between each measure.
 * param digitalPin is the digital pin that will be use for reading the digital output
 */
RainSensor::RainSensor(uint8_t analogPin, uint8_t digitalPin, void (*wet_CallBack)(void), void (*dry_CallBack)(void), uint8_t interval){
  this->analogPin = analogPin;
  this->digitalPin = digitalPin;
  this->interval = interval;
  this->analogValue = 0;
  this->previusMillis = millis();
  this->use = 2;
  this->wet_CallBack = wet_CallBack;
  this->dry_CallBack = dry_CallBack;
  this->isWet = false;
  this->isDry = false;
}
RainSensor::RainSensor(uint8_t analogPin, uint8_t interval){
  this->analogPin = analogPin;
  this->digitalPin = 20;
  this->interval = interval;
  this->analogValue = 0;
  this->previusMillis = millis();
  this->use = 1;
  this->wet_CallBack = 0;
  this->dry_CallBack = 0;
  this->isWet = false;
  this->isDry = false;
}
RainSensor::RainSensor(uint8_t digitalPin, void (*wet_CallBack)(void), void (*dry_CallBack)(void)){
  this->analogPin = 2;
  this->digitalPin = digitalPin;
  this->interval = 0;
  this->analogValue = -1;
  this->previusMillis = 0;
  this->wet_CallBack = wet_CallBack;
  this->dry_CallBack = dry_CallBack;
  this->use = 0;
  this->isWet = false;
  this->isDry = false;
}

void RainSensor::begin(){
  pinMode(digitalPin, INPUT);
}

/**
 *
 */
void RainSensor::sense(){
  #ifdef DEBUGGIN
    Serial.print("Sensing - Use" + String(use));
  #endif
  switch (use) {
    case 0:
          #ifdef DEBUGGIN
            Serial.println("Digital Only ..!!");
          #endif
          if(getDigitalState() == HIGH && isWet == false){
            #ifdef DEBUGGIN
              Serial.println("Fuction when WET is Active");
            #endif
            wet();
            isWet = true;
            isDry = false;
          }
          if(getDigitalState() == LOW && isDry == false){
            #ifdef DEBUGGIN
              Serial.println("Fuction when DRY is Active");
            #endif
            dry();
            isDry = true;
            isWet = false;
          }
    break;

    case 1:
          if(millis() - previusMillis > interval){
            analogValue = analogRead(analogPin);
            analogValue = map(analogValue,0,1023,0,2);
            previusMillis = millis();
          }
    break;

    case 2:
          if(digitalRead(digitalPin) == HIGH && isWet == false){
            wet_CallBack();
            isWet = true;
          }else{
            isWet = false;
          }
          if(digitalRead(digitalPin) == LOW && isDry == false){
            dry_CallBack();
            isDry = true;
          }else{
            isDry = false;
          }

          if(millis() - previusMillis > interval){
            analogValue = analogRead(analogPin);
            analogValue = map(analogValue,0,1023,0,2);
            previusMillis = millis();
          }
    break;
  }
}
