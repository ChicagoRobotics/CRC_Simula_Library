/***************************************************
Uses: Provides functions to Control the PCA9635 LED driver

This file is designed for the Simula project by Chicago Robotics Corp.
http://www.chicagorobotics.net/products

Copyright (c) 2016, Chicago Robotics Corp.
See README.md for license details
****************************************************/

#include "CRC_PCA9635.h"
#include <Wire.h>

#define PCA9635_REG_MODE1			0x00
#define PCA9635_REG_MODE2			0x01
#define PCA9635_REG_LEDBASE 		0x02
#define PCA9635_REG_LEDMODEBASE 	0x14

#define PCA9635_NUM_LED				16

#define PCA9635_AUTO_INCR       0x80


#define PCA9635_LEDOFF      0x00    // default @ startup
#define PCA9635_LEDON       0x01
#define PCA9635_LEDPWM      0x02
#define PCA9635_LEDGRPPWM   0x03

CRC_PCA9635::CRC_PCA9635(uint8_t address)
{
	_address = address;
}

void CRC_PCA9635::writeRegister(uint8_t regNum, byte* values, uint8_t size)
{
	Wire.beginTransmission(_address);
	Wire.write(regNum);
	Wire.write(values, size);
	Wire.endTransmission();
}

void CRC_PCA9635::writeRegister(uint8_t regNum, uint8_t value)
{
	Wire.beginTransmission(_address);
	Wire.write(regNum);
	Wire.write(value);
	Wire.endTransmission();
}

uint8_t CRC_PCA9635::readRegister(uint8_t reg)
{
	Wire.beginTransmission(_address);
	Wire.write(reg);
	Wire.endTransmission();
	if (Wire.requestFrom(_address, (uint8_t)1) != 1)
	{
		return 0;
	}
	int data = Wire.read();
	return data;
}

void CRC_PCA9635::init()
{
	// Init
	writeRegister(PCA9635_REG_MODE1, 0x81);  //  AUTOINCR | NOSLEEP | ALLADRR
											 // Set to PWM Mode for now
	for (int i = 0; i < 15; i++) {
		setLedMode(i, PCA9635_LEDPWM);
	}

	reset();

}

void CRC_PCA9635::reset()
{
	// TODO, Send Software Reset
	for (int i = 0; i < 15; i++) {
		setLed(i, 0);
	}
}

void CRC_PCA9635::setLedMode(uint8_t ledNum, uint8_t mode)
{
	if (ledNum <= 15 && mode <= 3)
	{
		uint8_t reg = PCA9635_REG_LEDMODEBASE + (ledNum >> 2);
		// some bit magic
		uint8_t shift = (ledNum & 0x03) * 2;  // 0,2,4,6 places
		uint8_t setmask = mode << shift;
		uint8_t clrmask = ~(0x03 << shift);
		uint8_t value = (readRegister(reg) & clrmask) | setmask;
		writeRegister(reg, value);
	}
}

void CRC_PCA9635::setLed(uint8_t ledNum, uint8_t level)
{
	writeRegister(PCA9635_REG_LEDBASE + ledNum, level);
}