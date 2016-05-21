/**
** Logger.cpp
** General purpose logger. Can log messages to Serial Port, Wireless
** As well as to a log file, based on configuration.
**/

#include "CRC_Logger.h"

LoggerClass Logger;

LoggerClass::LoggerClass()
{
	_initialized = false;
	_currentLevel = Logger.LOG_INFO;
	_totalDestinations = 0;
}

void LoggerClass::addLogDestination(Messaging *destination)
{
	_destinations[_totalDestinations++] = destination;
	_initialized = true;
}

void LoggerClass::log(uint8_t level, char* message)
{
	if (_currentLevel <= level && _initialized)
	{
		dispatch(message);
	}
}

void LoggerClass::log(uint8_t level, const char* message)
{
	if (_currentLevel <= level && _initialized)
	{
		dispatch((char * ) message);
	}
}

void LoggerClass::log(uint8_t level, const __FlashStringHelper* message)
{
	if (_currentLevel <= level && _initialized)
	{
		dispatch(message);
	}
}

void LoggerClass::logF(uint8_t level, const char* format, ...)
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

void LoggerClass::logF(uint8_t level, const  __FlashStringHelper* format, ...)
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


void LoggerClass::dispatch(char * message)
{
	// Dispatch the Log Message
	for (uint8_t i = 0; i < _totalDestinations; i++)
	{
		_destinations[i]->sendText(message);
	}
}

void LoggerClass::dispatch(const __FlashStringHelper* message)
{
	// Dispatch the Log Message
	for (uint8_t i = 0; i < _totalDestinations; i++)
	{
		_destinations[i]->sendText(message);
	}
}
