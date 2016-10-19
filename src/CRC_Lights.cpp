/***************************************************
Uses: Provides higher level functions to control Simula's LEDs.

This file is designed for the Simula project by Chicago Robotics Corp.
http://www.chicagorobotics.net/products

Copyright (c) 2016, Chicago Robotics Corp.
See README.md for license details
****************************************************/

#include "CRC_Lights.h"
#include "CRC_Hardware.h"
#define LIGHTS_LED_DEFINITION_COUNT     10

struct LIGHTS_LED_DEFINITION {
	boolean isLeft;
	uint8_t idxRed;
	uint8_t idxGreen;
	uint8_t idxBlue;
};
static const LIGHTS_LED_DEFINITION LIGHTS_LED_MAPPINGS[] = {
	{ true, 0x02, 0x01, 0x00 }, // 0 = L1
	{ true, 0x05, 0x04, 0x03 }, // 1 = L2
	{ true, 0x08, 0x07, 0x06 }, // 2 = L3
	{ true, 0x0B, 0x0A, 0x09 }, // 3 = L4
	{ true, 0x0E, 0x0D, 0x0C }, // 4 = L5

	{ false, 0x02, 0x01, 0x00 }, // 5 = R1
	{ false, 0x05, 0x04, 0x03 }, // 6 = R2
	{ false, 0x08, 0x07, 0x06 }, // 7 = R3
	{ false, 0x0B, 0x0A, 0x09 }, // 8 = R4
	{ false, 0x0E, 0x0D, 0x0C }  // 9 = R5
};

// Gamma Correction for RGB Linear Value
const uint8_t PROGMEM LIGHTS_LED_GAMMA[] =
{
	0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,
	1,  1,  1,  1,  1,  1,  1,  1,  1,  2,  2,  2,  2,  2,  2,  2,
	2,  3,  3,  3,  3,  3,  3,  3,  4,  4,  4,  4,  4,  5,  5,  5,
	5,  6,  6,  6,  6,  7,  7,  7,  7,  8,  8,  8,  9,  9,  9, 10,
	10, 10, 11, 11, 11, 12, 12, 13, 13, 13, 14, 14, 15, 15, 16, 16,
	17, 17, 18, 18, 19, 19, 20, 20, 21, 21, 22, 22, 23, 24, 24, 25,
	25, 26, 27, 27, 28, 29, 29, 30, 31, 32, 32, 33, 34, 35, 35, 36,
	37, 38, 39, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 50,
	51, 52, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 66, 67, 68,
	69, 70, 72, 73, 74, 75, 77, 78, 79, 81, 82, 83, 85, 86, 87, 89,
	90, 92, 93, 95, 96, 98, 99,101,102,104,105,107,109,110,112,114,
	115,117,119,120,122,124,126,127,129,131,133,135,137,138,140,142,
	144,146,148,150,152,154,156,158,160,162,164,167,169,171,173,175,
	177,180,182,184,186,189,191,193,196,198,200,203,205,208,210,213,
	215,218,220,223,225,228,231,233,236,239,241,244,247,249,252,255
};

const uint8_t PROGMEM aniRunway[5][5][3]= 
{
	{ //this means led 10 and 5 are on, the rest are off.
		{ 10, 5, 1 },
		{ 9, 4, 0 },
		{ 8, 3, 0 },
		{ 7, 2, 0 },
		{ 6, 1, 0 }
	},
	{ //led 9 and 4 are on, the rest are off, etc.
		{ 10, 5, 0 },
		{ 9, 4, 1 },
		{ 8, 3, 0 },
		{ 7, 2, 0 },
		{ 6, 1, 0 }
	},
	{
		{ 10, 5, 0 },
		{ 9, 4, 0 },
		{ 8, 3, 1 },
		{ 7, 2, 0 },
		{ 6, 1, 0 }
	},
	{
		{ 10, 5, 0 },
		{ 9, 4, 0 },
		{ 8, 3, 0 },
		{ 7, 2, 1 },
		{ 6, 1, 0 }
	},
	{
		{ 10, 5, 0 },
		{ 9, 4, 0 },
		{ 8, 3, 0 },
		{ 7, 2, 0 },
		{ 6, 1, 1 }
	}
};

CRC_LightsClass::CRC_LightsClass(uint8_t leftAddress, uint8_t rightAddress)
	:ledLeft(leftAddress), ledRight(rightAddress)
{
	allLedsOff = true;
}
void CRC_LightsClass::init()
{
	ledLeft.init();
	ledRight.init();
}
void CRC_LightsClass::setButtonLevel(uint8_t level) {
	analogWrite(hardware.pinButtonLED, level);
}
void CRC_LightsClass::showRunwayWithDelay() {

	for (int i = 0; i < 10; i++) {
		crcLights.setLed(i, 0, 0, 0);
	}

	for (int k = 0; k < 5; k++) {
		for (int j = 10; j > 4; j--) {
			for (int i = 0; i < 10; i++) {
				crcLights.setLed(i, 0, 0, 0);
			}

			crcLights.setLed(j, color_R, color_G, color_B);
			crcLights.setLed(j - 5, color_R, color_G, color_B);

			/*crcLights.setLed(j, 255, 0, 0);
			crcLights.setLed(j - 5, 255, 0, 0);*/
			delay(10);
		}
	}

	setAllOff();
}
void CRC_LightsClass::setAllOff() {
	if (!allLedsOff) {
		for (int i = 0; i < 10; i++) {
			crcLights.setLed(i, 0, 0, 0);
		}
		allLedsOff = true;
	}
}
void CRC_LightsClass::setRandomColor(){
	color_R = hardware.getRandomNumberInRange(0, 255);
	color_G = hardware.getRandomNumberInRange(0, 255);
	color_B = hardware.getRandomNumberInRange(0, 255);
}

/*
*Control the led channel mode, modes:
* 0 = fully off
* 1 = fully on(no PWM)
* 2 = individual PWM only
* 3 = individual and group PWM
*/
void CRC_LightsClass::setLed(CRC_PCA9635 & ledBank, uint8_t ledNum, uint8_t level)
{
	ledBank.setLed(ledNum, pgm_read_byte(&LIGHTS_LED_GAMMA[level]));
	if (level > 0)
	{
		allLedsOff = false;
	}
}
void CRC_LightsClass::setLeftLed(uint8_t ledNum, uint8_t level)
{
	setLed(ledLeft, ledNum, level);
}
void CRC_LightsClass::setRightLed(uint8_t ledNum, uint8_t level)
{
	setLed(ledRight, ledNum, level);
}
boolean CRC_LightsClass::setAllLeds(uint8_t red, uint8_t green, uint8_t blue)
{
	for (int i = 0; i < 10; i++) {
		crcLights.setLed(i, red, green, blue);
	}
	return true;
}
boolean CRC_LightsClass::setLed(uint8_t ledId, uint8_t red, uint8_t green, uint8_t blue)
{
	if (ledId >= LIGHTS_LED_DEFINITION_COUNT) {
		return false;
	}

	LIGHTS_LED_DEFINITION* mapping = (LIGHTS_LED_DEFINITION*) &(LIGHTS_LED_MAPPINGS[ledId]);

	if (mapping->isLeft) {
		setLed(ledLeft, mapping->idxRed, red);
		setLed(ledLeft, mapping->idxGreen, green);
		setLed(ledLeft, mapping->idxBlue, blue);
	}
	else {
		setLed(ledRight, mapping->idxRed, red);
		setLed(ledRight, mapping->idxGreen, green);
		setLed(ledRight, mapping->idxBlue, blue);
	}

	return true;
}
boolean CRC_LightsClass::setLedHex(uint8_t ledId, String hexString) {
	
	long number = strtol(&hexString[1], NULL, 16);
	long r = number >> 16;
	long g = number >> 8 & 0xFF;
	long b = number & 0xFF;
	
	setLed(ledId, r, g, b);

	return true;
}

