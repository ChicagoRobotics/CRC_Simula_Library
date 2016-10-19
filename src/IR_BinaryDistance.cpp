// 
// 
// 

#include "IR_BinaryDistance.h"

IR_BinaryDistance::IR_BinaryDistance(int activationPin, int readingPin)
	:DistanceSensor(activationPin, readingPin) {}

boolean IR_BinaryDistance::objectDetected() {
	boolean reading = false;
	int irValue = analogRead(_readingPin);
	if (irValue < 500)
	{
		reading = true;
	}
	return reading;
}