/***************************************************
Uses: Provides a higher level module around the sensors on
Simula

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

#include <Adafruit_LSM9DS0.h>

class CRC_Sensors {
protected:
	unsigned long lastIrPollSensors;
public:
	void init();
	void activate();
	void deactivate();
	void readIR();
	boolean irReadingUpdated();
	Adafruit_LSM9DS0 lsm;

	//Distance sensors
	boolean irLeftCliff;		// Left cliff sensor reading
	boolean irRightCliff;		// Right cliff sensor reading
	uint8_t irLeftCM;			// Left IR CM reading
	uint8_t irLeftFrontCM;		// Left front IR CM reading
	uint8_t irFrontCM;			// Front IR CM reading
	uint8_t irRightFrontCM;		// Right front IR CM reading
	uint8_t irRightCM;			// Right IR CM reading
	uint8_t pingFrontCM;		// Front Ping CM Reading
};

extern CRC_Sensors sensors;

#endif

