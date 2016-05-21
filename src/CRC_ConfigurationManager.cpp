// 
// 
// 
#include <SD.h>

#include "CRC_ConfigurationManager.h"
#include "CRC_Logger.h"
#include "CRC_FlashHelper.h"
#include "CRC_Globals.h"

ConfigurationManagerClass ConfigurationManager;

bool ConfigurationManagerClass::getConfig(const __FlashStringHelper * cfgName, char * szValue, size_t bufferSize)
{
	if (!initConfig())
	{
		return false;
	}
	File configFile = SD.open(FHM("/simula.cfg"));

	if (findConfig(configFile, cfgName))
	{
		readValue(configFile, szValue, bufferSize);
		configFile.close();
		return true;
	}
	configFile.close();
	return false;
}

bool ConfigurationManagerClass::getConfig(const char * cfgName, char * szValue, size_t bufferSize)
{
	if (!initConfig())
	{
		return false;
	}
	File configFile = SD.open(FHM("/simula.cfg"));

	if (findConfig(configFile, cfgName))
	{
		readValue(configFile, szValue, bufferSize);
		configFile.close();
		return true;
	}

	configFile.close();
	return false;
}

bool ConfigurationManagerClass::findConfig(File & configFile, const __FlashStringHelper* cfgName)
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

bool ConfigurationManagerClass::findConfig(File & configFile, const char* cfgName)
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

void ConfigurationManagerClass::readValue(File & configFile, char * szValue, size_t bufferSize)
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

uint32_t ConfigurationManagerClass::getConfigUint(const __FlashStringHelper* cfgName, uint32_t defaultValue, int base)
{
	char temp[15];
	if (getConfig(cfgName, temp, 20)) {
		return strtoul(temp, 0, base);
	}

	return defaultValue;
}

bool ConfigurationManagerClass::initConfig()
{
	if (SD.exists(FHM("/simula.cfg")))
	{
		return true;
	}

	if (!UnitState.sdCard) {
		return false;
	}

	Logger.log(Logger.LOG_WARN, F("Initializing new simula.cfg file"));
	File configFile = SD.open(FHM("/simula.cfg"), FILE_WRITE);
	configFile.write(FHM("# Simula Configuration\r\n"));
	configFile.close();
	
	return true;
}