// Sensors.h

#ifndef _SENSORS_h
#define _SENSORS_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

class SensorsClass
{
 protected:
	 uint16_t pingFrontMin;
	 uint16_t pingFrontSlope;
	 uint16_t pingFrontMax;

 protected:

	 void pollSensors();
	 void readAndSaveSensors();
	 void readAndSavePing();

	 boolean booleanIrCheck(int firstValue, int secondValue, int minTreshold);

	 unsigned long lastIrPollSensors;
	 int irLeft;
	 int irFrontLeft;
	 int irFrontRight;
	 int irRight;
	 int irFront;
	 int irFrontLeftEdge;
	 int irFrontRightEdge;

	 void debugLogBooleanSensor(const  __FlashStringHelper* message, boolean state);
 public:
	void init();

	void startScan();
	void endScan();
};

extern SensorsClass Sensors;

#endif

