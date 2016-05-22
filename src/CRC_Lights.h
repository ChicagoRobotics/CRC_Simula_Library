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

#include <pca9635.h>

class CRC_LightsClass
{
 private:
	 pca9635 ledLeft;
	 pca9635 ledRight;

	 inline void setLed(pca9635 & ledBank, uint8_t ledNum, uint8_t level);

 public:
	void init();

	void updateDisplayState(unsigned long now);  // Call this from the Loop function to update the Led display state

	void setLeftLed(uint8_t ledNum, uint8_t level); // 0=Off, 1000 = On, between 1-256 = Level
	void setRightLed(uint8_t ledNum, uint8_t level); // 0=Off, 1000 = On, between 1-256 = Level

	boolean setLed(uint8_t ledId, uint8_t red, uint8_t green, uint8_t blue);
};

extern CRC_LightsClass CRC_Lights;

#endif

