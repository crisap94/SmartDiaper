#include "Arduino.h"
#include "Fsr.h"

Fsr::Fsr(){
  this->analogPin = A0;
  this->numSamples = 200;
  this->delayTime = 40;
}

Fsr::Fsr(uint8_t analogPin,uint16_t numSamples, uint8_t delayTime){
  this->analogPin = analogPin;
  this->numSamples = numSamples;
  this->delayTime = delayTime;
}

uint16 Fsr::readRawValue() {
  uint16_t rawVal[numSamples];

  for (uint16_t i = 0; i < numSamples; i++) {
    rawVal[i] = analogRead(analogPin); // read the dust value via pin 5 on the sensor
    delayMicroseconds(delayTime);
  }
  // now we have the raw values, sort them
  sortData(rawVal, numSamples);
  // drop the lowest 25% and highest 25% of the readings
  uint16_t value = 0;
  for (uint16_t i = numSamples / 4; i < (numSamples * 3 / 4); i++) {
    value += rawVal[i];
  }
  value /= (numSamples / 2);
  return value;
}
// swap sort algorithm
void Fsr::sortData(uint16_t *arrayToBeSorted, uint16_t sizeOfArray){
  bool done = false;    // flag to know when we're done sorting
  uint16_t temp = 0;

  while(!done) {           // simple swap sort, sorts numbers from lowest to highest
    done = true;
    for (uint16_t j = 0; j < (sizeOfArray - 1); j++) {
      if (arrayToBeSorted[j] > arrayToBeSorted[j + 1]){     // numbers are out of order - swap
        temp = arrayToBeSorted[j + 1];
        arrayToBeSorted [j+1] =  arrayToBeSorted[j] ;
        arrayToBeSorted [j] = temp;
        done = false;
      }
    }
  }
}
