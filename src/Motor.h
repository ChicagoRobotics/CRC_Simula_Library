// Motor.h

#ifndef _MOTOR_h
#define _MOTOR_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#ifndef Encoder_h_
#include <Encoder.h>
#endif // !_ENCODER_h

class Motor : public Encoder {
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
	Motor(int encoderPin1, int encoderPin2, int mtrEnable, int mtrIn1, int mtrIn2);
	void setPower(int power);
	void stop();
	bool positionChanged();
	bool motorActive;
	void accelerateToEncoderTarget(int32_t encoderTarget, int powerTarget);
	//Set encoder pulses per second
	void setEncoderRate(int32_t pulsesPerSecond);
};

class Motors {
public:
	Motor* motorLeft;
	Motor* motorRight;
	void initialize(Motor* mtrLeft, Motor* mtrRight) {
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

extern Motors motors;

#endif

