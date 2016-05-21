// Motor.h

#ifndef _MOTOR_h
#define _MOTOR_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

class MotorClass
{
private:
	void setMotor(int mtrEnable, int mtrIn1, int mtrIn2, int intSpeed);

 public:
	 void setMotor(int leftMotor, int rightMotor);
};

extern MotorClass Motor;

#endif

