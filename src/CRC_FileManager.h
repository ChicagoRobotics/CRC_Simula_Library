// FileManager.h

#ifndef _FILEMANAGER_h
#define _FILEMANAGER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "CRC_MessageTypes.h"
class Messaging;

class FileManagerClass
{
 protected:
	 void handleLs(HIVEMSG_FILE_CONTROL * controlCommand, uint8_t messageSequence, uint8_t messageId, Messaging * messageSource);
	 void handleRm(HIVEMSG_FILE_CONTROL * controlCommand, uint8_t messageSequence, uint8_t messageId, Messaging * messageSource);
	 void handleMkdir(HIVEMSG_FILE_CONTROL * controlCommand, uint8_t messageSequence, uint8_t messageId, Messaging * messageSource);

	 void handleGet(HIVEMSG_FILE_CONTROL * controlCommand, uint8_t messageSequence, uint8_t messageId, Messaging * messageSource);
	 void handlePut(HIVEMSG_FILE_CONTROL * controlCommand, uint8_t messageSequence, uint8_t messageId, Messaging * messageSource);


 public:
	void handleMessage(uint8_t messageSequence, uint8_t messageId, void* messageContent, Messaging * messageSource);

};

extern FileManagerClass FileManager;

#endif

