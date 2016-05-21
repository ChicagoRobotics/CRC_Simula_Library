// 
// 
// 

#include "CRC_Sensors.h"
#include "CRC_Globals.h"
#include "CRC_Logger.h"
#include "CRC_Hardware.h"
#include "CRC_ConfigurationManager.h"

SensorsClass Sensors;

#define THRESHOLD_IR_BOOLEAN      200
#define PING_TIMEOUT              1000 * 100

void SensorsClass::init()
{
	lastIrPollSensors = 0;

	digitalWrite(Hardware.pinFrontPingTrigger, LOW);

	// Ping Configurations, defaults with overrides in the
	// Simula config file
	pingFrontMin = ConfigurationManager.getConfigUint(F("ping.zero"), 412);
	pingFrontSlope = ConfigurationManager.getConfigUint(F("ping.slope"), 58);
	pingFrontMax = ConfigurationManager.getConfigUint(F("ping.max"), 3355);
}

void SensorsClass::startScan()
{
	pollSensors();
}

void SensorsClass::endScan()
{
	pollSensors();

}

void SensorsClass::readAndSaveSensors()
{
	digitalWrite(Hardware.pinFrontPingTrigger, HIGH);

	irLeft = analogRead(Hardware.pinIrLeft);
	irFrontLeft = analogRead(Hardware.pinIrFrontLeft);
	irFrontRight = analogRead(Hardware.pinIrFrontRight);
	irRight = analogRead(Hardware.pinIrRight);
	irFront = analogRead(Hardware.pinIrFront);
	irFrontLeftEdge = analogRead(Hardware.pinIrFrontLeftEdge);
	irFrontRightEdge = analogRead(Hardware.pinIrFrontRightEdge);
	lastIrPollSensors = millis();

	// readAndSavePing();
}

void SensorsClass::readAndSavePing()
{
	// Read Ping Sensors
	digitalWrite(Hardware.pinFrontPingTrigger, LOW);
	UnitState.pingFrontRaw = pulseIn(Hardware.pinFrontPingEcho, PING_TIMEOUT);

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

void SensorsClass::pollSensors()
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
		Logger.logF(Logger.LOG_TRACE, F("Forced IR Read: %ul"), diff);
	}

	digitalWrite(Hardware.pinFrontPingTrigger, HIGH);
	UnitState.irLeft = !booleanIrCheck(irLeft, analogRead(Hardware.pinIrLeft), THRESHOLD_IR_BOOLEAN);
	UnitState.irFrontLeft = !booleanIrCheck(irFrontLeft, analogRead(Hardware.pinIrFrontLeft), THRESHOLD_IR_BOOLEAN);
	UnitState.irFrontRight = !booleanIrCheck(irFrontRight, analogRead(Hardware.pinIrFrontRight), THRESHOLD_IR_BOOLEAN);
	UnitState.irRight = !booleanIrCheck(irRight, analogRead(Hardware.pinIrRight), THRESHOLD_IR_BOOLEAN);
	UnitState.irFrontLeftEdge = booleanIrCheck(irFrontLeftEdge, analogRead(Hardware.pinIrFrontLeftEdge), THRESHOLD_IR_BOOLEAN);
	UnitState.irFrontRightEdge = booleanIrCheck(irFrontRightEdge, analogRead(Hardware.pinIrFrontRightEdge), THRESHOLD_IR_BOOLEAN);
	
	// TODO, Need to handle front IR sensors
	
	// readAndSavePing();

	// We have a complete read
	lastIrPollSensors = 0;
}

boolean SensorsClass::booleanIrCheck(int firstValue, int secondValue, int minTreshold)
{
	if (firstValue >= minTreshold && secondValue >= minTreshold) {
		return true;
	}

	return false;
}

void SensorsClass::debugLogBooleanSensor(const  __FlashStringHelper* message, boolean state)
{
	if (state)
	{
		Logger.log(Logger.LOG_INFO, message);
	}

}
