/***************************************************
Uses: Module for controlling and configuring the Zigbee module.
Currenty, this discovers and sets up the Zigbee Wifi module with
SSID roaming support. SSIDs can be stored in the configuration manager.

This file is designed for the Simula project by Chicago Robotics Corp.
http://www.chicagorobotics.net/products

Copyright (c) 2016, Chicago Robotics Corp.
See README.md for license details
****************************************************/

#ifndef _CRC_ZIGBEECONTROLLER_h
#define _CRC_ZIGBEECONTROLLER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include <HardwareSerial.h>
#include "CRC_StopWatch.h"
#include "CRC_Messaging.h"

class CRC_ZigbeeController
{
 protected:
	 HardwareSerial * _serialPort;
	 CRC_Messaging *  _wirelessComm;
	 unsigned long _baudRate;
	 boolean _isConnected;

	 boolean scanForModule();

	 boolean enterCommandMode();
	 void exitCommandMode();
	 
	 char _receive[255];

	 boolean connectToNetwork();
	 int8_t _attemptedNetwork;
	 CRC_StopWatch _lastAttempt;

 public:
	void init(HardwareSerial & serialPort, CRC_Messaging & wirelessComm);
	boolean isReady();
	inline boolean isModuleDetected() { return _baudRate > 0; }

	char * sendCommand(char * command, boolean atomic=true);
	char * getNetworkId(boolean atomic=true);
	boolean isConnected(boolean logIsConnectedMessage, boolean atomic=true);
};

#endif

