// Logger.h

#ifndef _LOGGER_h
#define _LOGGER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "CRC_Messaging.h"

class LoggerClass
{
 protected:
	 boolean _initialized;
	 uint8_t _currentLevel;

	 Messaging *		_destinations[2];
	 uint8_t			_totalDestinations;
	 inline void		dispatch(char * message);
	 inline void        dispatch(const __FlashStringHelper* message);
 public:
	LoggerClass();

	static const uint8_t LOG_TRACE = 0;
	static const uint8_t LOG_INFO = 1;
	static const uint8_t LOG_WARN = 2;
	static const uint8_t LOG_ERROR = 3;

	void addLogDestination(Messaging *destination);
	void setLevel(uint8_t level) { _currentLevel = level; }

	void log(uint8_t level, char* message);
	void log(uint8_t level, const char* message);
	void log(uint8_t level, const __FlashStringHelper* message);
	void logF(uint8_t level, const char* format, ...);
	void logF(uint8_t level, const  __FlashStringHelper* format, ...);

};

extern LoggerClass Logger;

#endif

