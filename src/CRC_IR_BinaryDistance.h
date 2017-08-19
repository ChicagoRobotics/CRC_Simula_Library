/***************************************************
Uses: Implementation of the IR Distance Sensors following the 
Distance Sensors API. 

This file is designed for the Simula project by Chicago Robotics Corp.
http://www.chicagorobotics.net/products

Copyright (c) 2016, Chicago Robotics Corp.
See README.md for license details
****************************************************/

#ifndef _CRC_IR_BINARYDISTANCE_h
#define _CRC_IR_BINARYDISTANCE_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include "CRC_DistanceSensor.h"

class CRC_IR_BinaryDistance : public CRC_DistanceSensor {
public:
	CRC_IR_BinaryDistance(int activationPin, int readingPin);
	boolean objectDetected();
};

#endif