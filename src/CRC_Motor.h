/***************************************************
Uses: Provides a module for working with the motors.

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

#include <Encoder.h>

class CRC_Motor : public Encoder {
private:
	int _mtrEnable;
	int _mtrIn1;
	int _mtrIn2;
	int32_t _previousPosition;
	
	unsigned long _previousRateCheckMillis;
	const long _rateCheckInterval = 40;
	int _stallPower;
	int _currentPower;
public:
	CRC_Motor(int encoderPin1, int encoderPin2, int mtrEnable, int mtrIn1, int mtrIn2);
	void setPower(int power);
	void stop();
	bool positionChanged();
	bool motorActive;
	void accelerateToEncoderTarget(int32_t encoderTarget, int powerTarget);
	//Set encoder pulses per second
	void setEncoderRate(int32_t pulsesPerSecond);
};

class CRC_Motors {
public:
	CRC_Motor* motorLeft;
	CRC_Motor* motorRight;
	void initialize(CRC_Motor* mtrLeft, CRC_Motor* mtrRight) {
		motorLeft = mtrLeft;
		motorRight = mtrRight;
	}
	//power range: 0-255
	void setPower(int powerLeft, int powerRight) {
		motorLeft->setPower(powerLeft);
		motorRight->setPower(powerRight);
	}
	void allStop() {
		motorLeft->stop();
		motorRight->stop();
	}
	bool active() {
		if (motorLeft->motorActive || motorRight->motorActive) {
			return true;
			Serial.println(F("motors active."));
		}
		else {
			return false;
		}
	}
};

extern CRC_Motors motors;

#endif

