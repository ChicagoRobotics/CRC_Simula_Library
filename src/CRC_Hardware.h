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

struct HARDWARE_STATE {
	uint16_t totalRam = 0;      // Total RAM in bytes
	uint16_t freeRam = 0;       // Free RAM in bytes
	int8_t leftMotor = 0;       // -100 -> 100
	int8_t rightMotor = 0;       // -100 -> 100	
	boolean sdCard = false;      // SD Card initialized/available state
	boolean audioPlayer = false; // Audi Player state
	boolean audioPlaying = false; // Is the audio player playing
	uint8_t wireless = 0x00;     // Wireless Status
	unsigned long loopLastTimeMillis = 0; // Last Time in millis
	unsigned long loopMinTimeMillis = 0;  // Min Time in millis
	unsigned long loopMaxTimeMillis = 0;  // Max Time in millis
};
extern struct HARDWARE_STATE hardwareState;

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
	const byte pinButtonLED = 13;
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
	const byte pinAmpGain0 = 37;
	const byte pinAmpGain1 = 40;
	const byte pinAmpEnable = 41;
	const byte sdcard_cs = 4; // SPI Chip Select for SD Card
	const byte pinBatt = A2;

	// IR Sensor pins
	const byte pinEdge1 = A0;
	const byte pinActEdge1 = 27;
	const byte pinEdge2 = A1;
	const byte pinActEdge2 = 31;
	const byte pinPerim1 = A4;
	const byte pinActPerim1 = 26;
	const byte pinPerim2 = A5;
	const byte pinActPerim2 = 28;
	const byte pinPerim3 = A6;
	const byte pinActPerim3 = 30;
	const byte pinPerim4 = A7;
	const byte pinActPerim4 = 32;
	const byte pinFrntIr = A8;
	const byte pinActFrntIR = 29;
	const byte irMinimumCM = 3;
	const float lowBatteryVoltage = 6.3;

	// Ping Sensors
	const byte pinPingEcho = 6;
	const byte pinPingTrigger = 7;

	// Free Pins with Breakouts
	// const byte pinSpeaker = 11; // Unused, Breakout to Daughter Board
	// Reserved Pins with Breakouts
	// const byte SPI_miso = 50; // Reserved - Standard Arduino SPI
	// const byte SPI_mosi = 51; // Reserved - Standard Arduino SPI
	// const byte SPI_clk = 52;  // Reserved - Standard Arduino SPI

	const uint8_t i2cPca9635Left = 0x00;
	const uint8_t i2cPca9635Right = 0x01;

#endif
	void init();
	void startScanStatus(unsigned long startTime);
	void endScanStatus(unsigned long startTime);
	void seedRandomGenerator();
	float readBatteryVoltage();
	int getRandomNumberInRange(int lowest, int highest);
	bool sdInitialized;
private:
	void setupPins();
	void setupI2C();
	void setupSPI();
};

extern CRC_HardwareClass hardware;

#endif

