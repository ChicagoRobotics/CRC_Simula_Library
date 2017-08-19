/***************************************************
Uses: Provides higher level functions to work with the Distance Sensors. This is 
the base class inherited by the sensors.

This file is designed for the Simula project by Chicago Robotics Corp.
http://www.chicagorobotics.net/products

Copyright (c) 2016, Chicago Robotics Corp.
See README.md for license details
****************************************************/

#ifndef _CRC_DISTANCESENSOR_h
#define _CRC_DISTANCESENSOR_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

class CRC_DistanceSensor {
public:
	int _readingPin;
	int _activationPin;
	inline virtual void activate();
	inline virtual void deactivate();
	CRC_DistanceSensor(int activationPin, int readingPin);
};

#endif
