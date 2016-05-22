/***************************************************
Uses: General purpose logger. Can log messages to Serial Port, Wireless
As well as to a log file, based on configuration.

This file is designed for the Simula project by Chicago Robotics Corp.
http://www.chicagorobotics.net/products

Copyright (c) 2016, Chicago Robotics Corp.
See README.md for license details
****************************************************/

#ifndef _CRC_LOGGER_h
#define _CRC_LOGGER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "CRC_Messaging.h"

class CRC_LoggerClass
{
 protected:
	 boolean _initialized;
	 uint8_t _currentLevel;

	 CRC_Messaging *	_destinations[2];
	 uint8_t			_totalDestinations;
	 inline void		dispatch(char * message);
	 inline void        dispatch(const __FlashStringHelper* message);
 public:
	 CRC_LoggerClass();

	static const uint8_t LOG_TRACE = 0;
	static const uint8_t LOG_INFO = 1;
	static const uint8_t LOG_WARN = 2;
	static const uint8_t LOG_ERROR = 3;

	void addLogDestination(CRC_Messaging *destination);
	void setLevel(uint8_t level) { _currentLevel = level; }

	void log(uint8_t level, char* message);
	void log(uint8_t level, const char* message);
	void log(uint8_t level, const __FlashStringHelper* message);
	void logF(uint8_t level, const char* format, ...);
	void logF(uint8_t level, const  __FlashStringHelper* format, ...);

};

extern CRC_LoggerClass CRC_Logger;

#endif

