// 
// 
// 

#include "IR_AnalogDistance.h"

IR_AnalogDistance::IR_AnalogDistance(int activationPin, int readingPin)
	: DistanceSensor(activationPin, readingPin) {}

double IR_AnalogDistance::readDistance() {
	int irValue = analogRead(_readingPin);
	double irDistance = 187754 * pow(irValue, -1.51);
	return irDistance;
}