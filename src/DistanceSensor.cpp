// 
// 
// 

#include "DistanceSensor.h"

DistanceSensor::DistanceSensor(int activationPin, int readingPin)
{
	_readingPin = readingPin;
	_activationPin = activationPin;
}

void DistanceSensor::activate() {
	pinMode(_activationPin, OUTPUT);
	digitalWrite(_activationPin, HIGH);
}

void DistanceSensor::deactivate() {
	pinMode(_activationPin, OUTPUT);
	digitalWrite(_activationPin, LOW);
}