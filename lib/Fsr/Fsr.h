#ifndef _FSR_H
#define _FSR_H

#include "Arduino.h"

class Fsr {
private:
  uint8_t analogPin;
  uint16_t numSamples;
  uint8_t delayTime;

public:
  Fsr();
  Fsr(uint8_t analogPin,uint16_t numSamples = 200, uint8_t delayTime = 40);

  uint16 readRawValue();

  void sortData(uint16_t *arrayToBeSorted, uint16_t sizeOfArray);


  inline void setNumSamples(uint16_t numSamples){
    this->numSamples = numSamples;
  }
  inline uint16_t getNumSamples(){
    return this->numSamples;
  }
  inline void setDelayTime(uint8_t delayTime){
    this->delayTime = delayTime;
  }
  inline uint8_t getDelayTime(){
    return this->delayTime;
  }
  inline uint16_t getValue(){
    return readRawValue();
  }
};

#endif
