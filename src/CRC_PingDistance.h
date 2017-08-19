/***************************************************
Uses: Implementation of the Ping Distance Sensors following the
Distance Sensors API.

This file is designed for the Simula project by Chicago Robotics Corp.
http://www.chicagorobotics.net/products

Copyright (c) 2016, Chicago Robotics Corp.
See README.md for license details
****************************************************/

#ifndef _CRC_PINGDISTANCE_h
#define _CRC_PINGDISTANCE_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include "CRC_DistanceSensor.h"

class CRC_PingDistance : public CRC_DistanceSensor {
public:
	CRC_PingDistance(int activationPin, int readingPin);
	float readDistance();
};

#endif