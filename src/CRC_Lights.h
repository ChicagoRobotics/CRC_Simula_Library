/***************************************************
Uses: Provides higher level functions to Control the LED outputs

This file is designed for the Simula project by Chicago Robotics Corp.
http://www.chicagorobotics.net/products

Copyright (c) 2016, Chicago Robotics Corp.
See README.md for license details
****************************************************/

#ifndef _CRC_LIGHTS_h
#define _CRC_LIGHTS_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

#include "CRC_PCA9635.h"

class CRC_Lights
{
 private:
	 CRC_PCA9635& ledLeft;
	 CRC_PCA9635& ledRight;

	 inline void setLed(CRC_PCA9635& ledBank, uint8_t ledNum, uint8_t level);

 public:
	 CRC_Lights(CRC_PCA9635& leftBank, CRC_PCA9635& rightBank);

	void init();

	void setLeftLed(uint8_t ledNum, uint8_t level); // 0=Off, 1000 = On, between 1-256 = Level
	void setRightLed(uint8_t ledNum, uint8_t level); // 0=Off, 1000 = On, between 1-256 = Level

	boolean setLed(uint8_t ledId, uint8_t red, uint8_t green, uint8_t blue);
};

#endif

