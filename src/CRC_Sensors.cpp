/***************************************************
Uses: Module for controlling and scanning the sensors.

This file is designed for the Simula project by Chicago Robotics Corp.
http://www.chicagorobotics.net/products

Copyright (c) 2016, Chicago Robotics Corp.
See README.md for license details
****************************************************/

#include "CRC_Sensors.h"
#include "CRC_Globals.h"
#include "CRC_Logger.h"
#include "CRC_Hardware.h"
#include "CRC_ConfigurationManager.h"


#define THRESHOLD_IR_BOOLEAN      200
#define PING_TIMEOUT              1000 * 100

void CRC_SensorsClass::init()
{
	lastIrPollSensors = 0;

	digitalWrite(CRC_Hardware.pinFrontPingTrigger, LOW);

	// Ping Configurations, defaults with overrides in the
	// Simula config file
	pingFrontMin = CRC_ConfigurationManager.getConfigUint(F("ping.zero"), 412);
	pingFrontSlope = CRC_ConfigurationManager.getConfigUint(F("ping.slope"), 58);
	pingFrontMax = CRC_ConfigurationManager.getConfigUint(F("ping.max"), 3355);
}

void CRC_SensorsClass::startScan()
{
	pollSensors();
}

void CRC_SensorsClass::endScan()
{
	pollSensors();

}

void CRC_SensorsClass::readAndSaveSensors()
{
	digitalWrite(CRC_Hardware.pinFrontPingTrigger, HIGH);

	irLeft = analogRead(CRC_Hardware.pinIrLeft);
	irFrontLeft = analogRead(CRC_Hardware.pinIrFrontLeft);
	irFrontRight = analogRead(CRC_Hardware.pinIrFrontRight);
	irRight = analogRead(CRC_Hardware.pinIrRight);
	irFront = analogRead(CRC_Hardware.pinIrFront);
	irFrontLeftEdge = analogRead(CRC_Hardware.pinIrFrontLeftEdge);
	irFrontRightEdge = analogRead(CRC_Hardware.pinIrFrontRightEdge);
	lastIrPollSensors = millis();

	// readAndSavePing();
}

void CRC_SensorsClass::readAndSavePing()
{
	// Read Ping Sensors
	digitalWrite(CRC_Hardware.pinFrontPingTrigger, LOW);
	UnitState.pingFrontRaw = pulseIn(CRC_Hardware.pinFrontPingEcho, PING_TIMEOUT);

	if (UnitState.pingFrontRaw > pingFrontMax) {
		// Treat the Ping as Out of Range (Infinite)
		UnitState.pingFrontCM = 0xffff; // Max uint16t
	}
	else if (UnitState.pingFrontRaw <= pingFrontMin) {
		UnitState.pingFrontCM = 0;
	} else {
		UnitState.pingFrontCM = (UnitState.pingFrontRaw - pingFrontMin) * pingFrontSlope;
	}
}

void CRC_SensorsClass::pollSensors()
{
	unsigned long now = millis();
	unsigned long diff = now - lastIrPollSensors;

	if (lastIrPollSensors == 0)
	{
		// First Read of Sensors - pre-debounce
		readAndSaveSensors();
		return;
	}

	if (diff < 50)
	{
		// No Need to poll again yet
		// Let's debounce over a period of Millis
		return; 
	}

	if (diff > 1000)
	{
		// We lost some loop time, force a blocking poll
		readAndSaveSensors();
		delay(5);
		CRC_Logger.logF(CRC_Logger.LOG_TRACE, F("Forced IR Read: %ul"), diff);
	}

	digitalWrite(CRC_Hardware.pinFrontPingTrigger, HIGH);
	UnitState.irLeft = !booleanIrCheck(irLeft, analogRead(CRC_Hardware.pinIrLeft), THRESHOLD_IR_BOOLEAN);
	UnitState.irFrontLeft = !booleanIrCheck(irFrontLeft, analogRead(CRC_Hardware.pinIrFrontLeft), THRESHOLD_IR_BOOLEAN);
	UnitState.irFrontRight = !booleanIrCheck(irFrontRight, analogRead(CRC_Hardware.pinIrFrontRight), THRESHOLD_IR_BOOLEAN);
	UnitState.irRight = !booleanIrCheck(irRight, analogRead(CRC_Hardware.pinIrRight), THRESHOLD_IR_BOOLEAN);
	UnitState.irFrontLeftEdge = booleanIrCheck(irFrontLeftEdge, analogRead(CRC_Hardware.pinIrFrontLeftEdge), THRESHOLD_IR_BOOLEAN);
	UnitState.irFrontRightEdge = booleanIrCheck(irFrontRightEdge, analogRead(CRC_Hardware.pinIrFrontRightEdge), THRESHOLD_IR_BOOLEAN);
	
	// TODO, Need to handle front IR sensors
	
	// readAndSavePing();

	// We have a complete read
	lastIrPollSensors = 0;
}

boolean CRC_SensorsClass::booleanIrCheck(int firstValue, int secondValue, int minTreshold)
{
	if (firstValue >= minTreshold && secondValue >= minTreshold) {
		return true;
	}

	return false;
}

void CRC_SensorsClass::debugLogBooleanSensor(const  __FlashStringHelper* message, boolean state)
{
	if (state)
	{
		CRC_Logger.log(CRC_Logger.LOG_INFO, message);
	}

}
