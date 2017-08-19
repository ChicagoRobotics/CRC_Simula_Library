/***************************************************
Uses: Provides higher level functions to work with the Distance Sensors. This is
the base class inherited by the sensors.

This file is designed for the Simula project by Chicago Robotics Corp.
http://www.chicagorobotics.net/products

Copyright (c) 2016, Chicago Robotics Corp.
See README.md for license details
****************************************************/

#include "CRC_DistanceSensor.h"

CRC_DistanceSensor::CRC_DistanceSensor(int activationPin, int readingPin)
{
	_readingPin = readingPin;
	_activationPin = activationPin;
}

void CRC_DistanceSensor::activate() {
	pinMode(_activationPin, OUTPUT);
	digitalWrite(_activationPin, HIGH);
}

void CRC_DistanceSensor::deactivate() {
	pinMode(_activationPin, OUTPUT);
	digitalWrite(_activationPin, LOW);
}