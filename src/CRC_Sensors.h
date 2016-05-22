/***************************************************
Uses: Module for controlling and scanning the sensors.

This file is designed for the Simula project by Chicago Robotics Corp.
http://www.chicagorobotics.net/products

Copyright (c) 2016, Chicago Robotics Corp.
See README.md for license details
****************************************************/

#ifndef _CRC_SENSORS_h
#define _CRC_SENSORS_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

class CRC_SensorsClass
{
 protected:
	 uint16_t pingFrontMin;
	 uint16_t pingFrontSlope;
	 uint16_t pingFrontMax;

 protected:

	 void pollSensors();
	 void readAndSaveSensors();
	 void readAndSavePing();

	 boolean booleanIrCheck(int firstValue, int secondValue, int minTreshold);

	 unsigned long lastIrPollSensors;
	 int irLeft;
	 int irFrontLeft;
	 int irFrontRight;
	 int irRight;
	 int irFront;
	 int irFrontLeftEdge;
	 int irFrontRightEdge;

	 void debugLogBooleanSensor(const  __FlashStringHelper* message, boolean state);
 public:
	void init();

	void startScan();
	void endScan();
};

extern CRC_SensorsClass CRC_Sensors;

#endif

