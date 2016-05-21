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

// ID of the UNIT/TODO will change from an id
uint8_t g_intUnitID = 0x01;

unsigned long lastMessageTime;		// Last Time a message was recieved by this Unit

struct HIVEMSG_STATE UnitState;		// Current state of the Unit
Messaging WirelessComm;				// For Remote/Wireless Access
Messaging TerminalComm;               // Local diagnostic access
ZigbeeController Zigbee;

void setup()
{
	// Pass the reference to the Serial Port we have UsbComm connector on.
	TerminalComm.initUSB(Serial, true);		  
	Logger.addLogDestination(&TerminalComm);

	Logger.log(Logger.LOG_INFO, F("Unit bootup sequence"));

	Hardware.init();
	Lights.init();

	Zigbee.init(Serial2, WirelessComm);
	Logger.addLogDestination(&WirelessComm);

	lastMessageTime = millis();

	Logger.log(Logger.LOG_INFO, F("Unit bootup completed"));
}


void loop()
{
	unsigned long now = millis();

	Hardware.startScanStatus(now);
	AudioManager.updateAudioState();
	Lights.display(now);

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

	Hardware.endScanStatus(now);
}
