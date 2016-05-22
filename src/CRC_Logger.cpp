/***************************************************
Uses: General purpose logger. Can log messages to Serial Port, Wireless
 As well as to a log file, based on configuration.

This file is designed for the Simula project by Chicago Robotics Corp.
http://www.chicagorobotics.net/products

Copyright (c) 2016, Chicago Robotics Corp.
See README.md for license details
****************************************************/

#include "CRC_Logger.h"

CRC_LoggerClass CRC_Logger;

CRC_LoggerClass::CRC_LoggerClass()
{
	_initialized = false;
	_currentLevel = CRC_Logger.LOG_INFO;
	_totalDestinations = 0;
}

void CRC_LoggerClass::addLogDestination(CRC_Messaging *destination)
{
	_destinations[_totalDestinations++] = destination;
	_initialized = true;
}

void CRC_LoggerClass::log(uint8_t level, char* message)
{
	if (_currentLevel <= level && _initialized)
	{
		dispatch(message);
	}
}

void CRC_LoggerClass::log(uint8_t level, const char* message)
{
	if (_currentLevel <= level && _initialized)
	{
		dispatch((char * ) message);
	}
}

void CRC_LoggerClass::log(uint8_t level, const __FlashStringHelper* message)
{
	if (_currentLevel <= level && _initialized)
	{
		dispatch(message);
	}
}

void CRC_LoggerClass::logF(uint8_t level, const char* format, ...)
{
	if (_currentLevel <= level && _initialized)
	{
		char _text[64];
		va_list argptr;
		va_start(argptr, format);
		vsprintf(_text, format, argptr);
		va_end(argptr);

		dispatch(_text);
	}
}

void CRC_LoggerClass::logF(uint8_t level, const  __FlashStringHelper* format, ...)
{
	if (_currentLevel <= level && _initialized)
	{
		char _text[64];
		va_list argptr;
		va_start(argptr, format);
		vsprintf_P(_text, (char *) format, argptr);
		va_end(argptr);

		dispatch(_text);
	}
}


void CRC_LoggerClass::dispatch(char * message)
{
	// Dispatch the Log Message
	for (uint8_t i = 0; i < _totalDestinations; i++)
	{
		_destinations[i]->sendText(message);
	}
}

void CRC_LoggerClass::dispatch(const __FlashStringHelper* message)
{
	// Dispatch the Log Message
	for (uint8_t i = 0; i < _totalDestinations; i++)
	{
		_destinations[i]->sendText(message);
	}
}
