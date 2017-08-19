/***************************************************
Uses: Implementation of the Analog Distance Sensors following the
Distance Sensors API.

This file is designed for the Simula project by Chicago Robotics Corp.
http://www.chicagorobotics.net/products

Copyright (c) 2016, Chicago Robotics Corp.
See README.md for license details
****************************************************/
#include "CRC_IR_AnalogDistance.h"

CRC_IR_AnalogDistance::CRC_IR_AnalogDistance(int activationPin, int readingPin)
	: CRC_DistanceSensor(activationPin, readingPin) {}

double CRC_IR_AnalogDistance::readDistance() {
	int irValue = analogRead(_readingPin);
	double irDistance = 187754 * pow(irValue, -1.51);
	return irDistance;
}