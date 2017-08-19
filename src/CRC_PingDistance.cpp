/***************************************************
Uses: Implementation of the Ping Distance Sensors following the
Distance Sensors API.

This file is designed for the Simula project by Chicago Robotics Corp.
http://www.chicagorobotics.net/products

Copyright (c) 2016, Chicago Robotics Corp.
See README.md for license details
****************************************************/

#include "CRC_PingDistance.h"

CRC_PingDistance::CRC_PingDistance(int activationPin, int readingPin)
	: CRC_DistanceSensor(activationPin, readingPin) {
	pinMode(activationPin, OUTPUT);
	pinMode(readingPin, INPUT);
}

float CRC_PingDistance::readDistance()
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