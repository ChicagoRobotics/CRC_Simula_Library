/***************************************************
Uses: Manages configuration persistence. this is
stored in the /simula.cfg file on the SD card as
a simple key-value properties file.

This file is designed for the Simula project by Chicago Robotics Corp.
http://www.chicagorobotics.net/products

Copyright (c) 2016, Chicago Robotics Corp.
See README.md for license details
****************************************************/

#include <SD.h>

#include "CRC_ConfigurationManager.h"
#include "CRC_Logger.h"
#include "CRC_FlashHelper.h"
#include "CRC_Globals.h"

CRC_ConfigurationManagerClass CRC_ConfigurationManager;

bool CRC_ConfigurationManagerClass::getConfig(const __FlashStringHelper * cfgName, char * szValue, size_t bufferSize)
{
	if (!initConfig())
	{
		return false;
	}
	File configFile = SD.open(CRC_FHM("/simula.cfg"));

	if (findConfig(configFile, cfgName))
	{
		readValue(configFile, szValue, bufferSize);
		configFile.close();
		return true;
	}
	configFile.close();
	return false;
}

bool CRC_ConfigurationManagerClass::getConfig(const char * cfgName, char * szValue, size_t bufferSize)
{
	if (!initConfig())
	{
		return false;
	}
	File configFile = SD.open(CRC_FHM("/simula.cfg"));

	if (findConfig(configFile, cfgName))
	{
		readValue(configFile, szValue, bufferSize);
		configFile.close();
		return true;
	}

	configFile.close();
	return false;
}

bool CRC_ConfigurationManagerClass::findConfig(File & configFile, const __FlashStringHelper* cfgName)
{
	configFile.seek(0);

	char  szCfgName[100];
	int next;
	int offset = 0;

	szCfgName[0] = 0x00;
	szCfgName[1] = 0x00;

	while ( (next = configFile.read())  >= 0)
	{
		if (next == '\n')
		{
			// Not found
			offset = 0;
			szCfgName[0] = 0x00;
			szCfgName[1] = 0x00;
		}
		else if (next == '=')
		{
			if (strcmp_P(szCfgName, (char *)cfgName) == 0)
			{
				return true; // Found
			}

			offset = 0;		
			szCfgName[0] = 0x00;
			szCfgName[1] = 0x00;
		}
		else if (isPrintable(next) && next != '\r' && next != ' ')
		{
			szCfgName[offset++] = next;
			szCfgName[offset+1] = 0x00;
		}
	}

	return false;
}

bool CRC_ConfigurationManagerClass::findConfig(File & configFile, const char* cfgName)
{
	configFile.seek(0);

	char  szCfgName[100];
	int next;
	int offset = 0;

	szCfgName[0] = 0x00;
	szCfgName[1] = 0x00;

	while ((next = configFile.read()) >= 0)
	{
		if (next == '\n')
		{
			// Not found
			offset = 0;
			szCfgName[0] = 0x00;
			szCfgName[1] = 0x00;
		}
		else if (next == '=')
		{
			if (strcmp(szCfgName, cfgName) == 0)
			{
				return true; // Found
			}

			offset = 0;
			szCfgName[0] = 0x00;
			szCfgName[1] = 0x00;
		}
		else if (isPrintable(next) && next != '\r' && next != ' ')
		{
			szCfgName[offset++] = next;
			szCfgName[offset + 1] = 0x00;
		}
	}

	return false;
}

void CRC_ConfigurationManagerClass::readValue(File & configFile, char * szValue, size_t bufferSize)
{
	int next;
	int offset = 0;

	szValue[0] = 0x00;
	szValue[1] = 0x01;

	while ((next = configFile.read()) >= 0 && offset < bufferSize)
	{
		if (next == '\n')
		{
			return;
		}
		else if (isPrintable(next) && next != '\r')
		{
			if (next == ' ' && offset == 0)
			{
				continue; // Trim leading spaces
			}
			szValue[offset++] = next;
			szValue[offset + 1] = 0x00;
		}
	}
}

uint32_t CRC_ConfigurationManagerClass::getConfigUint(const __FlashStringHelper* cfgName, uint32_t defaultValue, int base)
{
	char temp[15];
	if (getConfig(cfgName, temp, 20)) {
		return strtoul(temp, 0, base);
	}

	return defaultValue;
}

bool CRC_ConfigurationManagerClass::initConfig()
{
	if (SD.exists(CRC_FHM("/simula.cfg")))
	{
		return true;
	}

	if (!UnitState.sdCard) {
		return false;
	}

	CRC_Logger.log(CRC_Logger.LOG_WARN, F("Initializing new simula.cfg file"));
	File configFile = SD.open(CRC_FHM("/simula.cfg"), FILE_WRITE);
	configFile.write(CRC_FHM("# Simula Configuration\r\n"));
	configFile.close();
	
	return true;
}