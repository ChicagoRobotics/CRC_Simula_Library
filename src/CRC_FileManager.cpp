/***************************************************
Uses: Manages files on the SD card as based on remote
file management commands. For direct sd manipulation
use the SD library directly.

This file is designed for the Simula project by Chicago Robotics Corp.
http://www.chicagorobotics.net/products

Copyright (c) 2016, Chicago Robotics Corp.
See README.md for license details
****************************************************/

#include <SD.h>
#include "CRC_FileManager.h"
#include "CRC_Messaging.h"

#define FILE_TX_BUFFER_SZ		50
#define FILE_RX_BUFFER_SZ       50

void CRC_FileManagerClass::handleMessage(uint8_t messageSequence, uint8_t messageId, void* messageContent, CRC_Messaging * messageSource)
{
	HIVEMSG_FILE_CONTROL * controlCommand = (HIVEMSG_FILE_CONTROL *) messageContent;

	switch (controlCommand->cmd) {
	case HIVEMSG_FILE_CMD_LS:
		handleLs(controlCommand, messageSequence, messageId, messageSource);
		break;
	case HIVEMSG_FILE_CMD_RM:
		handleRm(controlCommand, messageSequence, messageId, messageSource);
		break;
	case HIVEMSG_FILE_CMD_MKDIR:
		handleMkdir(controlCommand, messageSequence, messageId, messageSource);
		break;
	case HIVEMSG_FILE_CMD_CAT:
		handleGet(controlCommand, messageSequence, messageId, messageSource);
		break;
	case HIVEMSG_FILE_CMD_GET:
		handleGet(controlCommand, messageSequence, messageId, messageSource);
		break;
	case HIVEMSG_FILE_CMD_PUT:
		handlePut(controlCommand, messageSequence, messageId, messageSource);
		break;

	case HIVEMSG_FILE_CMD_INFO:
		// Ignore/NOP
		break;
	default:
		// Unrecognized command
		messageSource->sendAck(messageSequence, messageId, HIVEMSG_ACK_FAILED, ERR_INVALID_PARAM);
	}
}

void CRC_FileManagerClass::handleLs(HIVEMSG_FILE_CONTROL * controlCommand, uint8_t messageSequence, uint8_t messageId, CRC_Messaging * messageSource)
{
	File dir;
	dir = (strlen(controlCommand->filePath) == 0 ? SD.open("/") : SD.open(controlCommand->filePath));

	if (!dir.isDirectory())
	{
		messageSource->sendAck(messageSequence, messageId, HIVEMSG_ACK_FAILED, ERR_INVALID_PARAM);
		dir.close();
		return;
	}

	dir.rewindDirectory();
	HIVEMSG_FILE_CONTROL nextFile;


	while (true) {
		nextFile.cmd = HIVEMSG_FILE_CMD_INFO;
		File entry = dir.openNextFile();
		if (!entry) {
			break;
		}

		if (entry.isDirectory()) {
			nextFile.fileType = HIVEMSG_FILETYPE_DIR;
			nextFile.fileSize = 0;
		}
		else {
			nextFile.fileType = HIVEMSG_FILETYPE_FILE;
			nextFile.fileSize = entry.size();
		}

		sprintf(nextFile.filePath, "%s", entry.name());
		entry.close();

		messageSource->sendMessage(HIVEMSG_FILE_CONTROL_ID, (uint8_t *) &nextFile, sizeof(HIVEMSG_FILE_CONTROL));
	}
	dir.close();
	return;
}


void CRC_FileManagerClass::handleGet(HIVEMSG_FILE_CONTROL * controlCommand, uint8_t messageSequence, uint8_t messageId, CRC_Messaging * messageSource)
{
	if (!SD.exists(controlCommand->filePath)) {
		messageSource->sendAck(messageSequence, messageId, HIVEMSG_ACK_FAILED, ERR_FILE_NOT_FOUND);
		return;
	}

	File file = SD.open(controlCommand->filePath);
	if (file.isDirectory())
	{
		messageSource->sendAck(messageSequence, messageId, HIVEMSG_ACK_FAILED, ERR_INVALID_PARAM);
		file.close();
		return;
	}
 
	HIVEMSG_FILE_STREAM nextFile;
	nextFile.messageId = messageId;
	nextFile.messageSequence = messageSequence;
	nextFile.fileSize = file.size();

	uint8_t fileBuffer[FILE_TX_BUFFER_SZ];
	uint16_t bytesread;

	/**
	// TODO File Size reported may not be actual, pre-scan the file for actual size
	// Right now, it appears to match. If it doesn't
	// uncomment this
	file.seek(0);
	nextFile.fileSize = 0;
	bytesread = file.read(fileBuffer, FILE_TX_BUFFER_SZ);
	while (bytesread > 0) {
		nextFile.fileSize += bytesread;
		bytesread = file.read(fileBuffer, FILE_TX_BUFFER_SZ);
	}
	**/

	messageSource->sendMessage(HIVEMSG_FILE_STREAM_ID, (uint8_t *)&nextFile, sizeof(HIVEMSG_FILE_STREAM));

	// Stream the file contents
	file.seek(0);
	bytesread = file.read(fileBuffer, FILE_TX_BUFFER_SZ);
	while (bytesread > 0) {
		messageSource->flush();
		messageSource->writeBytes(fileBuffer, bytesread);
		messageSource->flush();
		bytesread = file.read(fileBuffer, FILE_TX_BUFFER_SZ);
	}

	// EOF
}


void CRC_FileManagerClass::handleRm(HIVEMSG_FILE_CONTROL * controlCommand, uint8_t messageSequence, uint8_t messageId, CRC_Messaging * messageSource)
{
	if (!SD.exists(controlCommand->filePath)) 
	{
		messageSource->sendAck(messageSequence, messageId, HIVEMSG_ACK_FAILED, ERR_FILE_NOT_FOUND);
		return;
	}

	File file = SD.open(controlCommand->filePath);
	boolean isDirectory = file.isDirectory();
	file.close();

	if (isDirectory)
	{
		if (SD.rmdir(controlCommand->filePath))
		{
			messageSource->sendAck(messageSequence, messageId, HIVEMSG_ACK_OK);
		}
		else {
			messageSource->sendAck(messageSequence, messageId, HIVEMSG_ACK_FAILED);
		}
	}
	else {
		if(SD.remove(controlCommand->filePath)) 
		{
			messageSource->sendAck(messageSequence, messageId, HIVEMSG_ACK_OK);
		}
		else {
			messageSource->sendAck(messageSequence, messageId, HIVEMSG_ACK_FAILED);
		}
	}
}

void CRC_FileManagerClass::handleMkdir(HIVEMSG_FILE_CONTROL * controlCommand, uint8_t messageSequence, uint8_t messageId, CRC_Messaging * messageSource)
{
	if (!SD.exists(controlCommand->filePath))
	{
		if(SD.mkdir(controlCommand->filePath))
		{
			messageSource->sendAck(messageSequence, messageId, HIVEMSG_ACK_OK);
		}
		else {
			messageSource->sendAck(messageSequence, messageId, HIVEMSG_ACK_FAILED);
		}

		return;
	}

	// Exists, it it is a directory, we are ok
	File dir = SD.open(controlCommand->filePath);
	boolean isDir = dir.isDirectory();
	dir.close();

	if (isDir)
	{
		messageSource->sendAck(messageSequence, messageId, HIVEMSG_ACK_OK);
	}
	else
	{
		messageSource->sendAck(messageSequence, messageId, HIVEMSG_ACK_FAILED);
	}
}

void CRC_FileManagerClass::handlePut(HIVEMSG_FILE_CONTROL * controlCommand, uint8_t messageSequence, uint8_t messageId, CRC_Messaging * messageSource)
{
	if (SD.exists(controlCommand->filePath))
	{
		if (!SD.remove(controlCommand->filePath))
		{
			messageSource->sendAck(messageSequence, messageId, HIVEMSG_ACK_FAILED);
			return;
		}
	}

	File file = SD.open(controlCommand->filePath, FILE_WRITE);
	uint8_t fileBuffer[FILE_RX_BUFFER_SZ];
	uint16_t bytesread;
	uint32_t remainingBytes = controlCommand->fileSize;

	// Send the buffer/chunking size as part of the ready message
	messageSource->sendAck(messageSequence, messageId, HIVEMSG_ACK_READY, FILE_RX_BUFFER_SZ);
	while (remainingBytes > 0) {
		if (remainingBytes > FILE_RX_BUFFER_SZ)
		{
			bytesread = messageSource->readBytes(fileBuffer, FILE_RX_BUFFER_SZ);
		}
		else
		{
			bytesread = messageSource->readBytes(fileBuffer, remainingBytes);
		}

		remainingBytes -= bytesread;

		if (bytesread > 0)
		{
			file.write(fileBuffer, bytesread);

			// Send back count processed
			messageSource->writeBytes((uint8_t*) &bytesread, sizeof(bytesread));
			messageSource->flush();
		}
	}
	
	file.close();
	messageSource->sendAck(messageSequence, messageId, HIVEMSG_ACK_OK);
}