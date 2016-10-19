// 
// 
// 

#include "PingDistance.h"

PingDistance::PingDistance(int activationPin, int readingPin)
	: DistanceSensor(activationPin, readingPin) {
	pinMode(activationPin, OUTPUT);
	pinMode(readingPin, INPUT);
}

float PingDistance::readDistance()
{
	float duration, cm;

	//Setup specific to HC-SRO4
	digitalWrite(_activationPin, LOW);
	delayMicroseconds(2);
	digitalWrite(_activationPin, HIGH);
	delayMicroseconds(5);
	digitalWrite(_activationPin, LOW);

	//response
	duration = pulseIn(_readingPin, HIGH, 6300); //Timeout for 1 meter of echo time

	//convert sound speed to distance, 63 microseconds per CM
	cm = duration / 63;

	digitalWrite(_activationPin, HIGH);
	digitalWrite(_activationPin, LOW);
	return cm;
}