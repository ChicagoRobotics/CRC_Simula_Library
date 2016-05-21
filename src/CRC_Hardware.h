// Hardware.h

#ifndef _HARDWARE_h
#define _HARDWARE_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

class HardwareClass
{
 public:

// Define Pin's by Hardware Revisions
// Default to the ALPHA Units Pinouts
#ifndef _BOARD_VER_
#define _BOARD_VER_    ALPHA
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

#endif

	void init();
	void startScanStatus(unsigned long startTime);
	void endScanStatus(unsigned long startTime);

private:
	void setupPins();
	void setupI2C();
	void setupSPI();
};

extern HardwareClass Hardware;

#endif

