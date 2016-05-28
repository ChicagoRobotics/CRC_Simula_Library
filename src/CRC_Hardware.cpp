/***************************************************
Uses: Hardware abstraction layer and definitions for the
Simula Board. Provides the constant definitions for each
board revision, as well as module initializations.

This file is designed for the Simula project by Chicago Robotics Corp.
http://www.chicagorobotics.net/products

Copyright (c) 2016, Chicago Robotics Corp.
See README.md for license details
****************************************************/

#include <SPI.h>
#include <SD.h>
#include <Wire.h>

#include "CRC_Hardware.h"
#include "CRC_Globals.h"



void CRC_HardwareClass::init()
{
#if defined(__AVR_ATmega2560__) 
	UnitState.totalRam = 8192; // AT Mega
#endif
	UnitState.loopLastTimeMillis = 0; 
	UnitState.loopMinTimeMillis = 9999999;  
	UnitState.loopMaxTimeMillis = 0;

	//Get a random seed value from an unused pin
	randomSeed(analogRead(26));

	setupPins();

	setupSPI();
	setupI2C();

	UnitState.sdCard = SD.begin(sdcard_cs);

}



void CRC_HardwareClass::setupPins()
{
	//Set up audio amplifier and audio player
	pinMode(vs1053_dreq, INPUT);
	pinMode(vs1053_reset, OUTPUT);
	digitalWrite(vs1053_reset, LOW);

	pinMode(vs1053_dcs, OUTPUT);
	digitalWrite(vs1053_dcs, HIGH);

	pinMode(vs1053_cs, OUTPUT);
	digitalWrite(vs1053_cs, LOW);

	// SD Card
	pinMode(sdcard_cs, OUTPUT);
	digitalWrite(sdcard_cs, HIGH);

	// Setting amplifier gain low.
	pinMode(ampGain0, OUTPUT);
	pinMode(ampGain1, OUTPUT);
	pinMode(ampEnable, OUTPUT);
	digitalWrite(ampGain0, LOW);
	digitalWrite(ampGain1, LOW);
	digitalWrite(ampEnable, LOW);

	// 
	pinMode(pinLED, OUTPUT);
	pinMode(pinButton, INPUT_PULLUP);

	// Motor Pins
	pinMode(mtr1In1, OUTPUT);
	pinMode(mtr1In2, OUTPUT);
	pinMode(mtr1Enable, OUTPUT);
	pinMode(mtr2In1, OUTPUT);
	pinMode(mtr2In2, OUTPUT);
	pinMode(mtr2Enable, OUTPUT);

	// Sensor Pins
	pinMode(pinIrLeft, INPUT);
	pinMode(pinIrFrontLeft, INPUT);
	pinMode(pinIrFrontRight, INPUT);
	pinMode(pinIrRight, INPUT);
	pinMode(pinIrFront, INPUT);
	pinMode(pinIrFrontLeftEdge, INPUT);
	pinMode(pinIrFrontRightEdge, INPUT);

	// Ping Pins
	pinMode(pinFrontPingTrigger, OUTPUT);
	pinMode(pinFrontPingEcho, INPUT);
}

void CRC_HardwareClass::setupSPI()
{
	SPI.begin();
	SPI.setDataMode(SPI_MODE0);
	SPI.setBitOrder(MSBFIRST);
	SPI.setClockDivider(SPI_CLOCK_DIV128);
}
void CRC_HardwareClass::setupI2C()
{
	Wire.begin();
	Wire.setTimeout(500);
}


void CRC_HardwareClass::startScanStatus(unsigned long startTime)
{
	// Scan Free Ram START
	// Keep this block as is at start of this method
	extern int __heap_start, *__brkval;
	int v;
	UnitState.freeRam = (uint16_t)(&v - (__brkval == 0 ? (uint16_t)&__heap_start : (uint16_t)__brkval));
	// Scan Free Ram END
};

void CRC_HardwareClass::endScanStatus(unsigned long startTime)
{
	unsigned long endTime = millis();
	unsigned long loopTime = endTime - startTime;
	UnitState.loopLastTimeMillis = loopTime; // Last Time in millis
	UnitState.loopMinTimeMillis = min(UnitState.loopMinTimeMillis + 1, loopTime);  // Min Time in millis
	UnitState.loopMaxTimeMillis = max(UnitState.loopMaxTimeMillis, loopTime);  // Max Time in millis
}


