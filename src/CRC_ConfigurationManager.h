/***************************************************
Uses: Manages configuration persistence. this is
stored in the /simula.cfg file on the SD card as 
a simple key-value properties file.

This file is designed for the Simula project by Chicago Robotics Corp.
http://www.chicagorobotics.net/products

Copyright (c) 2016, Chicago Robotics Corp.
See README.md for license details
****************************************************/


#ifndef _CRC_CONFIGURATIONMANAGER_h
#define _CRC_CONFIGURATIONMANAGER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include <SD.h>

class CRC_ConfigurationManagerClass
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

extern CRC_ConfigurationManagerClass CRC_ConfigurationManager;

#endif

