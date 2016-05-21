
#include "CRC_MessageTypes.h"

typedef void(*Message_Handler)(uint8_t messageSequence, uint8_t messageId, void* messageContent, Messaging * messageSource);

// Mapping between Message Id's and their associated Handlers
// The index in the Array is the Message Id, and the 
// Callback is the Callback
// Use NULL to ignore this message Id
static const Message_Handler MESSAGE_HANDLER[] = {
	msgHandler_SetMotors,  // HIVEMSG_MOVE_ID
	msgHandler_Query,      // HIVEMSG_QUERY_ID
	NULL,				   // HIVEMSG_ACK_ID
	NULL,                  // HIVEMSG_TEXT_ID
	NULL,                  // HIVEMSG_STATE
	msgHandler_MusicPlay,  // HIVEMSG_MUSIC_PLAY_ID
	msgHandler_MusicCtrl,  // HIVEMSG_MUSIC_CONTROL_ID
	msgHandler_LedCtrl,    // HIVEMSG_LED_CONTROL_ID
	msgHandler_File,       // HIVEMSG_FILE_CONTROL_ID
	NULL                   // HIVEMSG_FILE_STREAM_ID
};
#define MSG_HANDLER_SIZE	9

/**
* Handles any inbound messages from the passed in message source
*/
boolean handleMessages(Messaging * messageSource)
{
	boolean hadMessage = false;
	if (messageSource -> hasMessage())
	{
		hadMessage = true;

		uint8_t * message = messageSource->getMessageBuffer();
		uint8_t messageSequence = message[0];
		uint8_t messageId = message[1];

		if (messageId > MSG_HANDLER_SIZE)
		{
			// Unsupported Message Received
			messageSource->sendAck(messageSequence, messageId, HIVEMSG_ACK_UNKNOWN);
		} if (MESSAGE_HANDLER[messageId] == NULL)
		{
			// NOP, Do nothing with this message
		}
		else
		{
			// Delegate the Message to its handler
			MESSAGE_HANDLER[messageId](messageSequence, messageId, &(message[2]), messageSource); // Invoke the message handler
		}

		messageSource -> clearBuffer();
	}

	return hadMessage;
}

/**
* Handles the call to set the Motors, delegate to the Motor Manager
**/
void msgHandler_SetMotors(uint8_t messageSequence, uint8_t messageId, void * messageContent, Messaging * messageSource)
{
	HIVEMSG_MOVE * moveCommand = (HIVEMSG_MOVE *)messageContent;
	Motor.setMotor(moveCommand->leftMotor, moveCommand->rightMotor);

	// Acknowledge the Message
	messageSource->sendAck(messageSequence, messageId, HIVEMSG_ACK_OK);
}

/**
* Handles the call to query for information
**/
void msgHandler_Query(uint8_t messageSequence, uint8_t messageId, void * messageContent, Messaging * messageSource)
{
	HIVEMSG_QUERY * queryCommand = (HIVEMSG_QUERY *)messageContent;

	switch (queryCommand->queryType) {
	case 0x00:
		messageSource->sendMessage(HIVEMSG_STATE_ID, (uint8_t *) &UnitState, sizeof(HIVEMSG_STATE));
		break;
	default:
		messageSource->sendAck(messageSequence, messageId, HIVEMSG_ACK_UNKNOWN);
	}
}


void msgHandler_MusicPlay(uint8_t messageSequence, uint8_t messageId, void * messageContent, Messaging * messageSource)
{
	HIVEMSG_MUSIC_PLAY * playCommand = (HIVEMSG_MUSIC_PLAY *)messageContent;

	if (!SD.exists(playCommand->filePath))
	{
		messageSource->sendAck(messageSequence, messageId, HIVEMSG_ACK_FAILED, ERR_FILE_NOT_FOUND);
	} else if (!UnitState.audioPlayer)
	{
		messageSource->sendAck(messageSequence, messageId, HIVEMSG_ACK_FAILED, ERR_NOTAVAILABLE);
	} else if (!UnitState.audioPlayer)
	{
		messageSource->sendAck(messageSequence, messageId, HIVEMSG_ACK_FAILED, ERR_NOTAVAILABLE);
	}
	else if (playCommand->playType == HIVEMSG_MUSIC_CMD_PLAY)
	{
		AudioManager.startAudioFile(playCommand->filePath);
		messageSource->sendAck(messageSequence, messageId, HIVEMSG_ACK_OK);
	}
	else if (playCommand->playType == HIVEMSG_MUSIC_CMD_REQUEST)
	{
		if (AudioManager.isPlayingAudio()) {
			messageSource->sendAck(messageSequence, messageId, HIVEMSG_ACK_FAILED, ERR_BUSY);
		}
		else {
			AudioManager.startAudioFile(playCommand->filePath);
			messageSource->sendAck(messageSequence, messageId, HIVEMSG_ACK_OK);
		}
	}
	else {
		messageSource->sendAck(messageSequence, messageId, HIVEMSG_ACK_FAILED, ERR_INVALID_PARAM);
	}

}

void msgHandler_MusicCtrl(uint8_t messageSequence, uint8_t messageId, void * messageContent, Messaging * messageSource)
{
	HIVEMSG_MUSIC_CONTROL * controlCommand = (HIVEMSG_MUSIC_CONTROL *)messageContent;

	switch(controlCommand->cmd) {
		case HIVEMSG_MUSIC_CMD_STOP:
			AudioManager.stopAudio();
			messageSource->sendAck(messageSequence, messageId, HIVEMSG_ACK_OK);
			break;
		case HIVEMSG_MUSIC_CMD_VOLUME:
			AudioManager.setVolume(controlCommand->value1, controlCommand->value2);
			messageSource->sendAck(messageSequence, messageId, HIVEMSG_ACK_OK);
			break;
		case HIVEMSG_MUSIC_CMD_AMP:
			AudioManager.setAmpGain(controlCommand->value1);
			messageSource->sendAck(messageSequence, messageId, HIVEMSG_ACK_OK);
			break;
		default:
			messageSource->sendAck(messageSequence, messageId, HIVEMSG_ACK_FAILED, ERR_INVALID_PARAM);
	}
}

void msgHandler_LedCtrl(uint8_t messageSequence, uint8_t messageId, void * messageContent, Messaging * messageSource)
{
	HIVEMSG_LED_CONTROL * controlCommand = (HIVEMSG_LED_CONTROL *)messageContent;

	switch (controlCommand->cmd) {
	case HIVEMSG_LED_CMD_SET:
		Lights.setLed(controlCommand->ledId, controlCommand->red, controlCommand->green, controlCommand->blue);
		messageSource->sendAck(messageSequence, messageId, HIVEMSG_ACK_OK);
		break;
	default:
		messageSource->sendAck(messageSequence, messageId, HIVEMSG_ACK_FAILED, ERR_INVALID_PARAM);
	}
}

void msgHandler_File(uint8_t messageSequence, uint8_t messageId, void * messageContent, Messaging * messageSource)
{
	// Delegate for now, eventually we want to register the modules directly
	FileManager.handleMessage(messageSequence, messageId, messageContent, messageSource);

}
