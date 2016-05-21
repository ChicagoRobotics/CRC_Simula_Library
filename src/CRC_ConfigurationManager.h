/**
* ConfigurationManager.h
* Manages configuration persistence. Currently this is
* stored in the /.config file on the SD card.
**/ 

#ifndef _CONFIGURATIONMANAGER_h
#define _CONFIGURATIONMANAGER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include <SD.h>

class ConfigurationManagerClass
{
 private:
	 bool findConfig(File & configFile, const __FlashStringHelper* cfgName);
	 bool findConfig(File & configFile, const char * cfgName);
	 void readValue(File & configFile, char * szValue, size_t bufferSize);
	 bool initConfig();
 public:
	 bool getConfig(const __FlashStringHelper* cfgName, char * szValue, size_t bufferSize);
	 bool getConfig(const char* cfgName, char * szValue, size_t bufferSize);

	 uint32_t getConfigUint(const __FlashStringHelper* cfgName, uint32_t defaultValue, int base=10);
	 
	 // void setConfig(const char * cfgName, char * szValue);
};

extern ConfigurationManagerClass ConfigurationManager;

#endif

