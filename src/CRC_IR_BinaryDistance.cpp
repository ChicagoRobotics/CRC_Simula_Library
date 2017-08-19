/***************************************************
Uses: Implementation of the IR Distance Sensors following the
Distance Sensors API.

This file is designed for the Simula project by Chicago Robotics Corp.
http://www.chicagorobotics.net/products

Copyright (c) 2016, Chicago Robotics Corp.
See README.md for license details
****************************************************/

#include "CRC_IR_BinaryDistance.h"

CRC_IR_BinaryDistance::CRC_IR_BinaryDistance(int activationPin, int readingPin)
	:CRC_DistanceSensor(activationPin, readingPin) {}

boolean CRC_IR_BinaryDistance::objectDetected() {
	boolean reading = false;
	int irValue = analogRead(_readingPin);
	if (irValue < 500)
	{
		reading = true;
	}
	return reading;
}