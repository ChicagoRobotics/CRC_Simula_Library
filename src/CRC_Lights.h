/***************************************************
Uses: Provides higher level functions to Control the LED outputs

This file is designed for the Simula project by Chicago Robotics Corp.
http://www.chicagorobotics.net/products

Copyright (c) 2016, Chicago Robotics Corp.
See README.md for license details
****************************************************/

#ifndef _LIGHTS_h
#define _LIGHTS_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "CRC_PCA9635.h"

class CRC_LightsClass
{

private:
	CRC_PCA9635 ledLeft;
	CRC_PCA9635 ledRight;
	inline void setLed(CRC_PCA9635 & ledBank, uint8_t ledNum, uint8_t level);
	boolean allLedsOff;
	
public:
	CRC_LightsClass(uint8_t leftAddress, uint8_t rightAddress);
	uint8_t color_R;
	uint8_t color_G;
	uint8_t color_B;
	void init();
	void setLeftLed(uint8_t ledNum, uint8_t level); // 0=Off, 1000 = On, between 1-256 = Level
	void setRightLed(uint8_t ledNum, uint8_t level); // 0=Off, 1000 = On, between 1-256 = Level
	void showRunwayWithDelay();
	void setButtonLevel(uint8_t level);
	void setAllOff();
	void setRandomColor();
	
	boolean setLed(uint8_t ledId, uint8_t red, uint8_t green, uint8_t blue);
	boolean setAllLeds(uint8_t red, uint8_t green, uint8_t blue);
	boolean setLedHex(uint8_t ledId, String hexString);
};

extern CRC_LightsClass crcLights;

#endif

