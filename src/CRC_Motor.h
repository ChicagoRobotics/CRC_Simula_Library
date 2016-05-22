/***************************************************
Uses: Module for controlling and interacting with the motors.

This file is designed for the Simula project by Chicago Robotics Corp.
http://www.chicagorobotics.net/products

Copyright (c) 2016, Chicago Robotics Corp.
See README.md for license details
****************************************************/

#ifndef _CRC_MOTOR_h
#define _CRC_MOTOR_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

class CRC_MotorClass
{
private:
	void setMotor(int mtrEnable, int mtrIn1, int mtrIn2, int intSpeed);

 public:
	 void setMotor(int leftMotor, int rightMotor);
};

extern CRC_MotorClass CRC_Motor;

#endif

