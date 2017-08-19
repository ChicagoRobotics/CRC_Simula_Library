/***************************************************
Uses: Provides a higher level module around the sensors on
Simula

This file is designed for the Simula project by Chicago Robotics Corp.
http://www.chicagorobotics.net/products

Copyright (c) 2016, Chicago Robotics Corp.
See README.md for license details
****************************************************/

#include "CRC_Sensors.h"
#include "CRC_IR_BinaryDistance.h"
#include "CRC_IR_AnalogDistance.h"
#include "CRC_PingDistance.h"
#include "CRC_Hardware.h"

void Sensors::init() {
	lsm = Adafruit_LSM9DS0();
}

void Sensors::activate() {
	//Activate sensors
	digitalWrite(hardware.pinActEdge1, HIGH);
	digitalWrite(hardware.pinActEdge2, HIGH);
	digitalWrite(hardware.pinActPerim1, HIGH);
	digitalWrite(hardware.pinActPerim2, HIGH);
	digitalWrite(hardware.pinActPerim3, HIGH);
	digitalWrite(hardware.pinActPerim4, HIGH);
	digitalWrite(hardware.pinActFrntIR, HIGH);
	lastIrPollSensors = 0;
}

void Sensors::deactivate() {
	//Activate sensors
	digitalWrite(hardware.pinActEdge1, LOW);
	digitalWrite(hardware.pinActEdge2, LOW);
	digitalWrite(hardware.pinActPerim1, LOW);
	digitalWrite(hardware.pinActPerim2, LOW);
	digitalWrite(hardware.pinActPerim3, LOW);
	digitalWrite(hardware.pinActPerim4, LOW);
	digitalWrite(hardware.pinActFrntIR, LOW);
	lastIrPollSensors = 0;
}

void Sensors::readIR() {
	CRC_IR_BinaryDistance edgeLeft = CRC_IR_BinaryDistance(hardware.pinActEdge1, hardware.pinEdge1);
	CRC_IR_BinaryDistance edgeRight = CRC_IR_BinaryDistance(hardware.pinActEdge2, hardware.pinEdge2);
	CRC_IR_AnalogDistance perimLeft = CRC_IR_AnalogDistance(hardware.pinActPerim1, hardware.pinPerim1);
	CRC_IR_AnalogDistance perimLeftFront = CRC_IR_AnalogDistance(hardware.pinActPerim2, hardware.pinPerim2);
	CRC_IR_AnalogDistance perimFront = CRC_IR_AnalogDistance(hardware.pinActFrntIR, hardware.pinFrntIr);
	CRC_IR_AnalogDistance perimRightFront = CRC_IR_AnalogDistance(hardware.pinActPerim3, hardware.pinPerim3);
	CRC_IR_AnalogDistance perimRight = CRC_IR_AnalogDistance(hardware.pinActPerim4, hardware.pinPerim4);
	CRC_PingDistance frontPing = CRC_PingDistance(hardware.pinPingTrigger, hardware.pinPingEcho);

	sensors.irLeftCM = perimLeft.readDistance();
	sensors.irLeftFrontCM = perimLeftFront.readDistance();
	sensors.irFrontCM = perimFront.readDistance();
	sensors.irRightFrontCM = perimRightFront.readDistance();
	sensors.irRightCM = perimRight.readDistance();
	sensors.pingFrontCM = frontPing.readDistance();

	//If there is no object detected, then we MAY have a cliff.
	sensors.irLeftCliff = !edgeLeft.objectDetected();
	sensors.irRightCliff = !edgeRight.objectDetected();
	
	lastIrPollSensors = millis();
}

boolean Sensors::irReadingUpdated() {
	unsigned long now = millis();
	unsigned long diff = now - lastIrPollSensors;

	if (lastIrPollSensors == 0)
	{
		// First Read of Sensors - pre-debounce
		return false;
	}

	if (diff < 50)
	{
		// 50 ms is still a fresh reading
		return true;
	}

	if (diff > 1200)
	{
		Serial.println(F("Long loop, forcing sensor read."));
		return false;
		//CRC_Logger.logF(CRC_Logger.LOG_TRACE, F("Forced IR Read: %ul"), diff);
	}
	return false;
}
