/***************************************************
Uses: Provides functions to Control the PCA9635 LED driver

This file is designed for the Simula project by Chicago Robotics Corp.
http://www.chicagorobotics.net/products

Copyright (c) 2016, Chicago Robotics Corp.
See README.md for license details
****************************************************/

#ifndef _PCA_9635_h
#define _PCA_9635_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

class CRC_PCA9635
{
protected:

	uint8_t _address;

	/**
	* Sets the Mode
	**/
	void setLedMode(uint8_t ledNum, uint8_t mode);

	void writeRegister(uint8_t regNum, byte* values, uint8_t size);
	void writeRegister(uint8_t regNum, uint8_t value);
	uint8_t readRegister(uint8_t reg);

public:
	CRC_PCA9635(uint8_t address);

	void init();
	void reset();
	/**
	* Sets the LED to the specified Level (0=Off, 255=On)
	*/
	void setLed(uint8_t ledNum, uint8_t level);
};

#endif

