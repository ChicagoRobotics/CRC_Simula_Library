/***************************************************
Uses: Provides a module for working with the motors.

This file is designed for the Simula project by Chicago Robotics Corp.
http://www.chicagorobotics.net/products

Copyright (c) 2016, Chicago Robotics Corp.
See README.md for license details
****************************************************/

#include "CRC_Motor.h"

CRC_Motor::CRC_Motor(int encoderPin1, int encoderPin2, int mtrEnable, int mtrIn1, int mtrIn2)
	: Encoder(encoderPin1, encoderPin2) {
	_mtrEnable = mtrEnable;
	_mtrIn1 = mtrIn1;
	_mtrIn2 = mtrIn2;
	_previousPosition = 0;
	_previousRateCheckMillis = 0;
	_stallPower = 0;
	pinMode(_mtrEnable, OUTPUT);
	pinMode(_mtrIn1, OUTPUT);
	pinMode(_mtrIn2, OUTPUT);
	stop();
	motorActive = false;
}

void CRC_Motor::setPower(int power) {
	boolean in1 = HIGH;
	boolean in2 = LOW;

	if (power > 255) {
		Serial.print(F("Power max exceeded.  Power: "));
		Serial.println(power);
	}
	else {
		if (power > 0)
		{
			in1 = LOW;
			in2 = HIGH;
			motorActive = true;
		}
		else {
			motorActive = false;
		}
		analogWrite(_mtrEnable, abs(power));
		digitalWrite(_mtrIn1, in1);
		digitalWrite(_mtrIn2, in2);
	}
}

void CRC_Motor::stop() {
	analogWrite(_mtrEnable, 0);
	digitalWrite(_mtrIn1, LOW);
	digitalWrite(_mtrIn2, LOW);
	motorActive = false;
}

bool CRC_Motor::positionChanged() {
	bool _positionChanged = false;
	int32_t _newPosition = read();
	if (_newPosition != _previousPosition) {
		_previousPosition = _newPosition;
		_positionChanged = true;
	}
	return _positionChanged;
}

void CRC_Motor::accelerateToEncoderTarget(int32_t encoderTarget, int powerTarget) {
	unsigned long _currentMillis = millis();
	if (read() < encoderTarget) {
		if (_currentMillis - _previousRateCheckMillis >= _rateCheckInterval) {
			_previousRateCheckMillis = _currentMillis;
			if (!positionChanged()) {
				_stallPower += 5;
				Serial.print(F("Stall power: "));
				Serial.println(_stallPower);
			}
		}
		setPower(_stallPower);
	}
	else
	{
		stop();
	}
}

void CRC_Motor::setEncoderRate(int32_t pulsesPerSecond) {
	//1000 millis per second
	int _interval = 20;
	int _convertedPulses = pulsesPerSecond * _interval / 1000;
	unsigned long _currentMillis = millis();
	if (_currentMillis - _previousRateCheckMillis >= _interval) {
		_previousRateCheckMillis = _currentMillis;
		int32_t _currentPosition = read();
		int32_t _pulsesPerInterval = _currentPosition - _previousPosition;
		_previousPosition = _currentPosition;
		if (_pulsesPerInterval < _convertedPulses) {
			_currentPower += 1;
			setPower(_currentPower);
		}
		if (_pulsesPerInterval > _convertedPulses)
		{
			_currentPower -= 1;
			setPower(_currentPower);
		}
		Serial.print(F("Current power: "));
		Serial.println(_currentPower);
		Serial.print(F("measuredPPS: "));
		Serial.println(_pulsesPerInterval * 1000 / _interval);
		Serial.print(F("targetPPS: "));
		Serial.println(pulsesPerSecond);
	}

}
