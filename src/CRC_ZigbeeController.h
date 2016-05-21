// ZigbeeController.h

#ifndef _ZIGBEECONTROLLER_h
#define _ZIGBEECONTROLLER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include <HardwareSerial.h>
#include "CRC_StopWatch.h"
#include "CRC_Messaging.h"

class ZigbeeController
{
 protected:
	 HardwareSerial * _serialPort;
	 Messaging *      _wirelessComm;
	 unsigned long _baudRate;
	 boolean _isConnected;

	 boolean scanForModule();

	 boolean enterCommandMode();
	 void exitCommandMode();
	 
	 char _receive[255];

	 boolean connectToNetwork();
	 int8_t _attemptedNetwork;
	 StopWatch _lastAttempt;

 public:
	void init(HardwareSerial & serialPort, Messaging & wirelessComm);
	boolean isReady();
	inline boolean isModuleDetected() { return _baudRate > 0; }

	char * sendCommand(char * command, boolean atomic=true);
	char * getNetworkId(boolean atomic=true);
	boolean isConnected(boolean logIsConnectedMessage, boolean atomic=true);
};

#endif

