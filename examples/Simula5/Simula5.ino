/***************************************************
This file is designed for the Simula project by Chicago Robotics Corp.
http://www.chicagorobotics.net/products

Copyright (c) 2016, Chicago Robotics Corp.
See README.md for license details
****************************************************/


#include <SD.h>							// By Arduino/Spark Fun
#include <HardwareSerial.h>
#include <SPI.h>						// Arduino Lib  
#include <pca9635.h>					// TODO Replace Me
#include <i2c_device.h>                 // TODO Replace Me with 2-Wire
#include <I2C.h>                        // TODO Replace Me with 2-Wire

#include "CRC_FlashHelper.h"
#include "CRC_ConfigurationManager.h"
#include "CRC_Logger.h"
#include "CRC_AudioManager.h"
#include "CRC_Hardware.h"
#include "CRC_Globals.h"
#include "CRC_MessageTypes.h"
#include "CRC_Lights.h"
#include "CRC_Messaging.h"
#include "CRC_ZigbeeController.h"
#include "CRC_Sensors.h"
#include "CRC_FileManager.h"
#include "CRC_Motor.h"

unsigned long lastMessageTime;		// Last Time a message was recieved by this Unit

struct HIVEMSG_STATE UnitState;		// Current state of the Unit
CRC_Messaging WirelessComm;			// For Remote/Wireless Access
CRC_Messaging TerminalComm;         // Local diagnostic access
CRC_ZigbeeController Zigbee;

void setup()
{
	// Pass the reference to the Serial Port we have UsbComm connector on.
	TerminalComm.initUSB(Serial, true);		  
	CRC_Logger.addLogDestination(&TerminalComm);

	CRC_Logger.log(CRC_Logger.LOG_INFO, F("Unit bootup sequence"));

	CRC_Hardware.init();
	CRC_Lights.init();

	Zigbee.init(Serial2, WirelessComm);
	CRC_Logger.addLogDestination(&WirelessComm);

	lastMessageTime = millis();

	CRC_Logger.log(CRC_Logger.LOG_INFO, F("Unit bootup completed"));
}


void loop()
{
	unsigned long now = millis();

	CRC_Hardware.startScanStatus(now);
	CRC_AudioManager.updateAudioState();
	CRC_Lights.updateDisplayState(now);

	// Start::Wireless
	if (Zigbee.isReady())
	{
		// We also need to check for Wireless hasMessages
		if (handleMessages(&WirelessComm))
		{
			lastMessageTime = now;
		}
	}
	// End::Wireless

	// Start::Diagnostic/Usb Port
	if (handleMessages(&TerminalComm))
	{
		lastMessageTime = now;
	}
	// End::Diagnostic/Usb Port

	CRC_Hardware.endScanStatus(now);
}
