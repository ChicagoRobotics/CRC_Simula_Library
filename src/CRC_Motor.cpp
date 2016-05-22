/***************************************************
Uses: Module for controlling and interacting with the motors.

This file is designed for the Simula project by Chicago Robotics Corp.
http://www.chicagorobotics.net/products

Copyright (c) 2016, Chicago Robotics Corp.
See README.md for license details
****************************************************/

#include "CRC_Motor.h"
#include "CRC_Globals.h"
#include "CRC_Hardware.h"

CRC_MotorClass CRC_Motor;


void CRC_MotorClass::setMotor(int mtrEnable, int mtrIn1, int mtrIn2, int intSpeed) {
	boolean in1 = HIGH;
	boolean in2 = LOW;

	if (intSpeed > 0)
	{
		in1 = LOW;
		in2 = HIGH;
	}

	// We get 0-100. We want to scale from minimal speed without stalling
	// to maximum speed, but stalling is a function of voltage, torque etc.
	// For now map anything above 0 To 156-255.
	// Anything below 155, map to 0
	intSpeed = abs(intSpeed) + 155;
	if (intSpeed <= 155)
	{
		intSpeed = 0;
	}

	analogWrite(mtrEnable, intSpeed);
	digitalWrite(mtrIn1, in1);
	digitalWrite(mtrIn2, in2);
}

void CRC_MotorClass::setMotor(int leftMotor, int rightMotor)
{
	setMotor(CRC_Hardware.mtr2Enable, CRC_Hardware.mtr2In1, CRC_Hardware.mtr2In2, rightMotor);
	setMotor(CRC_Hardware.mtr1Enable, CRC_Hardware.mtr1In1, CRC_Hardware.mtr1In2, leftMotor);

	UnitState.leftMotor = leftMotor;
	UnitState.rightMotor = rightMotor;
}

