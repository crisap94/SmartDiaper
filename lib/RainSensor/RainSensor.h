#ifndef _RAINSENSOR_H
#define _RAINSENSOR_H

#include "Arduino.h"



class RainSensor{
private:
  uint8_t analogPin; //Define the analog pin that will be used
  uint8_t digitalPin; //Define the digital pin which is going to read the digital output of the sensor
  uint8_t interval; //It is the interval of the analog measure
  uint16_t analogValue; //Is the analog Value
  uint32_t previusMillis; // Is the time used for matchng a correct interval
  uint8_t use; // determine wich mode of the sensor will be used 0-Digital-Only  1-Analog-only   2-Digital-Analog
  bool isWet;
  bool isDry;

  void (*wet_CallBack)(void);
  void (*dry_CallBack)(void);


public:
  //The number of the pins doesn't match because you can define the use of the sensor
  RainSensor(){};
  RainSensor(uint8_t digitalPin, void (*wet_CallBack)(void) = 0, void (*dry_CallBack)(void) = 0);
  RainSensor(uint8_t analogPin , uint8_t interval = 120);
  RainSensor(uint8_t analogPin , uint8_t digitalPin,  void (*wet_CallBack)(void), void (*dry_CallBack)(void) = 0, uint8_t interval = 120);

  void begin();
  void sense();

  inline uint16_t getAnalogValue(){
    return this->analogValue;
  }

  inline bool getDigitalState(){
    return !digitalRead(digitalPin);
  }

  inline void wet() {
    if (wet_CallBack){
      (*wet_CallBack)();
    }
	}

  inline void dry() {
    if (dry_CallBack){
      (*dry_CallBack)();
    }
	}


};


#endif
