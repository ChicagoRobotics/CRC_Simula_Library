/***************************************************
Uses: Hardware abstraction layer and definitions for the
Simula Board. Provides the constant definitions for each
board revision, as well as module initializations.

This file is designed for the Simula project by Chicago Robotics Corp.
http://www.chicagorobotics.net/products

Copyright (c) 2016, Chicago Robotics Corp.
See README.md for license details
****************************************************/
#ifndef _CRC_HARDWARE_h
#define _CRC_HARDWARE_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

class CRC_HardwareClass
{
 public:

// Define Pin's by Hardware Revisions
// Default to the ALPHA Units Pinouts
#ifndef _CRC_BOARD_VER_
#define _CRC_BOARD_VER_    ALPHA
	 const byte enc1A = 3;
	 const byte enc1B = 2;
	 const byte pinButton = 5;
	 const byte pinLED = 13;
	 const byte enc2A = 18;
	 const byte enc2B = 19;

	 const byte mtr1In1 = 24;
	 const byte mtr1In2 = 22;
	 const byte mtr1Enable = 44;

	 const byte mtr2In1 = 23;
	 const byte mtr2In2 = 25;
	 const byte mtr2Enable = 45;

	 // VS1053 Control
	 const byte vs1053_dcs = 8;
	 const byte vs1053_reset = 9; 
	 const byte vs1053_cs = 10;   // SPI CS for VS 1053 Control Port
	 const byte vs1053_dreq = 12; // SPI CS for VS 1053 Data Port
	 const byte ampGain0 = 37;
	 const byte ampGain1 = 40;
	 const byte ampEnable = 41;

	 const byte sdcard_cs = 4; // SPI Chip Select for SD Card

	 const byte pinBatt = A2;

	 // IR Sensors
	 const byte pinIrLeft = A7;
	 const byte pinIrFrontLeft = A6; 
	 const byte pinIrFrontRight = A5;
	 const byte pinIrRight = A4;
	 const byte pinIrFront = A8;
	 const byte pinIrFrontLeftEdge = A1;
	 const byte pinIrFrontRightEdge = A0;

	 // Ping Sensors
	 const byte pinFrontPingEcho = 6;
	 const byte pinFrontPingTrigger = 7;

	 // Free Pins with Breakouts
	 // const byte pinSpeaker = 11; // Unused, Breakout to Daughter Board

	 // Reserved Pins with Breakouts
	 // const byte SPI_miso = 50; // Reserved - Standard Arduino SPI
	 // const byte SPI_mosi = 51; // Reserved - Standard Arduino SPI
	 // const byte SPI_clk = 52;  // Reserved - Standard Arduino SPI

	 const uint8_t i2cPca9635Left = 0x00;
	 const uint8_t i2cPca9635Right= 0x01;
#endif

	void init();
	void startScanStatus(unsigned long startTime);
	void endScanStatus(unsigned long startTime);

private:
	void setupPins();
	void setupI2C();
	void setupSPI();
};

extern CRC_HardwareClass CRC_Hardware;

#endif

