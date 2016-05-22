/***************************************************
Uses: Manages files on the SD card as based on remote
file management commands. For direct sd manipulation
use the SD library directly.

This file is designed for the Simula project by Chicago Robotics Corp.
http://www.chicagorobotics.net/products

Copyright (c) 2016, Chicago Robotics Corp.
See README.md for license details
****************************************************/

#ifndef _CRC_FILEMANAGER_h
#define _CRC_FILEMANAGER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "CRC_MessageTypes.h"
class CRC_Messaging;

class CRC_FileManagerClass
{
 protected:
	 void handleLs(HIVEMSG_FILE_CONTROL * controlCommand, uint8_t messageSequence, uint8_t messageId, CRC_Messaging * messageSource);
	 void handleRm(HIVEMSG_FILE_CONTROL * controlCommand, uint8_t messageSequence, uint8_t messageId, CRC_Messaging * messageSource);
	 void handleMkdir(HIVEMSG_FILE_CONTROL * controlCommand, uint8_t messageSequence, uint8_t messageId, CRC_Messaging * messageSource);

	 void handleGet(HIVEMSG_FILE_CONTROL * controlCommand, uint8_t messageSequence, uint8_t messageId, CRC_Messaging * messageSource);
	 void handlePut(HIVEMSG_FILE_CONTROL * controlCommand, uint8_t messageSequence, uint8_t messageId, CRC_Messaging * messageSource);


 public:
	void handleMessage(uint8_t messageSequence, uint8_t messageId, void* messageContent, CRC_Messaging * messageSource);

};

extern CRC_FileManagerClass CRC_FileManager;

#endif

