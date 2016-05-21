/**
** Messaging.cpp
** Note: We could use # define here to separate the 802.15.4 vs 802.11b/g/n
** code if we want to save prog-space. I.E. Flash a unit with the specific support
** it needs.
**/

#include "CRC_Messaging.h"
#include "CRC_Globals.h"
#include "CRC_Logger.h"

#define XBEE_START				'*'

Messaging::Messaging()
{
	_isEnabled = false;
}

/**
* Initializes to use the Binary communications protocol
**/
void Messaging::initUSB(HardwareSerial & serialPort, boolean enabled)
{
	_serialPort = &serialPort;
	_serialPort->begin(115200, SERIAL_8N1);
	_readStatus = COM_START;
	_isEnabled = enabled;
}


/**
* Initializes to use the Wifi communications protocol which is all we have right now
**/
void Messaging::initZigbeeWifi(HardwareSerial & serialPort, boolean enabled)
{
	_serialPort = &serialPort;
	_readStatus = COM_START;
	_isEnabled = enabled;
}

/**
* Reads the serial port until we have a complete command. Then it returns true.
* This function implements a FSM. 
* Commands need to start with a command delimiter
**/
bool Messaging::hasMessage()
{
	return readMessage();
}

/**
* Clears the current data buffer. Discards any messages currently read.
**/
void Messaging::clearBuffer()
{
	memset(_chrDataBuffer, 0, sizeof(_chrDataBuffer));
	_bufferPos = 0;
	_calcChecksum = 0;
	_readStatus = COM_START;
}

/**
* Implements the Wirless Zigbee Message Protocol. This is the current protocol
* that relies on the unit/modules robot id.
* Return true if the unit has read a completed command.
* Note: The data buffer has a max size of 255, messages should not exceed that 
*
* Inbound Message Format
* [*]		The * delimiter to indicate the start of a message
* [unitId]  Unsigned char indicating the id of the unit the message is intended for
* [len]     Unsigned char indicating the length of the message to follow (excluding checksum).
* [msg]     Message characters
* [chksum]  Unsigned char indicating the checksum of the message. 
**/
bool Messaging::readMessage()
{
	if (!_isEnabled) {
		return false;
	}

	int read;
	// Serial.print("Current Status: ");
	// Serial.println(_readStatus, DEC);

	switch (_readStatus) 
	{
	case COM_START:
		// Wait for the Message Start Character
		while (_serialPort->available() > 0)
		{
			if (_serialPort->read() == XBEE_START)
			{
				_readStatus = COM_ADDRESS;
				break; // We have message start, read/wait for Message Address
			}
		}

		if (_readStatus != COM_ADDRESS)
		{
			return false;
		}
		// We are in COM ADDRESS State, fall to next case statement and continue reading
	case COM_ADDRESS:
		// Serial.println("Check for Message Address");
		if (_serialPort->available() > 0)
		{
			// TODO, UID Header should go away
			// USB Comm does not need UID
			// Wifi Comm relies on UDP Broadcasts
			// Or Direct IP Address.
			read = _serialPort->read();
			if (read >= 0)
			{
				// We have message start, intended for this unit
				_readStatus = COM_TRANSFER; 
				_bufferPos = 0;
				_expectedMsgLength = 0;
				_calcChecksum = 0;
			}
			else
			{
				// We are not the recepient/skip until the next frame
				_readStatus = COM_START;

				return false;
			}
		}
		
		if (_readStatus != COM_TRANSFER)
		{
			return false;
		}

		// We are in COM_TRANSFER state, fall to next case statement and continue reading
	case COM_TRANSFER:
		// First byte to follow is the expected length
		if (_expectedMsgLength == 0)
		{
			if (_serialPort->available() >= 2)
			{
				_serialPort->readBytes((char *) &_expectedMsgLength, 2);
				if (_expectedMsgLength == 0) {
					// Why did we receive a message of 0 length?
					_readStatus = COM_START;
					return false;
				}
				else if (_expectedMsgLength > 255)
				{
					// Our internal buffer is capped at 255, discard this message, its too large
					// to handle
					_readStatus = COM_START;
					return false;
				}

				_expectedMsgLength += 1; // Read the message + checksum

				// Serial.print("Expected Message Length: ");
				// Serial.println(_expectedMsgLength, DEC);
			}
			else
				return false;
		}

		while (_serialPort->available() > 0)
		{
			_messageChecksum = _serialPort->read(); // Could be checksum, or next data char
			_chrDataBuffer[_bufferPos ++] = _messageChecksum;
			if (_bufferPos == _expectedMsgLength)
			{
				_readStatus = COM_COMPLETED;
				_calcChecksum -= _chrDataBuffer[0]; // Do not include sequence
				_calcChecksum -= _chrDataBuffer[1]; // Do not include msg id
				_calcChecksum &= 0xff;
				break;
			}
			else
			{
				_calcChecksum += _messageChecksum; // Not the checksum, was a data byte
			}
		}

		if (_readStatus != COM_COMPLETED)
		{
			return false;
		}

	case COM_COMPLETED:
		// We have a completed message if the checksums match
		if (_calcChecksum == _messageChecksum)
		{
			_chrDataBuffer[_bufferPos - 1] = 0; // Terminate at the point of the checksum
			return true;
		}
		else
		{
			// Serial.print("Bad Message: Checksum");
			// Serial.print(_calcChecksum, DEC);
			// Serial.print(", Expected Checksum");
			// Serial.println(_messageChecksum, DEC);
			Logger.logF(Logger.LOG_ERROR, F("Bad Checksum, Calc: %d,  Expected Checksum, %d"), _calcChecksum, _messageChecksum);

			// Checksum mismatch, discard this message
			clearBuffer();
			return false;
		}
	}

	return false;
}

/**
* Sends messages to the receiver. This is assuming a single reciever. How are we handling destination
* addresses?
*/
void Messaging::sendMessage(uint8_t messageId, uint8_t * messageBuffer, uint16_t messageLength)
{
	if (!_isEnabled) {
		return;
	}
	sendMessageImpl(messageId, messageBuffer, messageLength);
}

/**
* Flush any pending output messages
*/
void Messaging::flush()
{
	_serialPort->flush();
}

/**
* Implements the Wirless Zigbee Message Protocol. This is the current protocol
* that relies on the unit/modules robot id.
* Note: The data buffer has a max size of 255, messages should not exceed that
*
* Outbound Message Format
* [*]		The * delimiter to indicate the start of a message
* [unitId]  Unsigned char indicating the id of the unit sending the message
* [len]     Unsigned int16 indicating the length of the message to follow (including checksum).
* [msg]     Message characters
* [chksum]  Unsigned char indicating the checksum of the message.
**/
void Messaging::sendMessageImpl(uint8_t messageId, uint8_t * messageBuffer, uint16_t messageLength)
{
	// Make sure any previous messages have been sent
	_serialPort->flush(); 

	// Send the message
	_messageSequence++;
	uint16_t actualLength = messageLength + 2; // Include sequence and messageId

	_serialPort->print(F("*"));
	_serialPort->write(g_intUnitID);
	_serialPort->write((uint8_t*) &actualLength, 2);
	_serialPort->write(_messageSequence);
	_serialPort->write(messageId);
	_serialPort->write(messageBuffer, messageLength);
	_serialPort->write(calculateChecksum(messageBuffer, messageLength));

	// Block and wait until the message has been sent
	_serialPort->flush(); 
}


/**
* Calculates the single byte checksum for the passed in buffer
**/
uint8_t Messaging::calculateChecksum(uint8_t * messageBuffer, uint16_t messageLength)
{
	uint8_t checksum = 0;
	for (uint16_t i = 0; i < messageLength; i++)
	{
		checksum += messageBuffer[i];
	}
	checksum &= 0xff;
	return checksum;
}

/**
* Sends the ack message in response to a recieved message
**/
void Messaging::sendAck(uint8_t messageSequence, uint8_t messageId, uint8_t ackCode, uint8_t ackParam)
{
	flush(); // Ensure we don't have any pending messages, since we reuse the msg structure
	_ackMsg.sequence = messageSequence;
	_ackMsg.messageId = messageId;
	_ackMsg.ackCode = ackCode;
	_ackMsg.ackParam = ackParam;
	sendMessage(HIVEMSG_ACK_ID, (uint8_t *)&_ackMsg, sizeof(HIVEMSG_ACK));
}

/**
* Sends Text messages, generally used for diagnostics.
**/
void Messaging::sendText(const char* message)
{
	flush(); // Ensure we don't have any pending messages, since we reuse the msg structure
	strncpy(_textMsg.text, message, 60);
	sendMessage(HIVEMSG_TEXT_ID, (uint8_t *)&_textMsg, sizeof(HIVEMSG_TEXT));
}

/**
* Sends Text messages, generally used for diagnostics.
**/
void Messaging::sendText(const __FlashStringHelper* message)
{
	flush(); // Ensure we don't have any pending messages, since we reuse the msg structure
	strncpy_P(_textMsg.text, (char *)message, 60);
	sendMessage(HIVEMSG_TEXT_ID, (uint8_t *)&_textMsg, sizeof(HIVEMSG_TEXT));
}

/**
* Sends formatted text messages, generally used for diagnostics. This delegates to 
* sprintf so caveats apply. If you want to sent floats/doubles, create a String() object,
* append the float and send the float as text instead.
**/
void Messaging::sendTextF(const char* format, ...)
{
	flush(); // Ensure we don't have any pending messages, since we reuse the msg structure
	va_list argptr;
	va_start(argptr, format);
	vsprintf(_textMsg.text, format, argptr);
	va_end(argptr);
	sendMessage(HIVEMSG_TEXT_ID, (uint8_t *)&_textMsg, sizeof(HIVEMSG_TEXT));
}

/**
* Sends formatted text messages, generally used for diagnostics. This delegates to
* sprintf so caveats apply. If you want to sent floats/doubles, create a String() object,
* append the float and send the float as text instead.
**/
void Messaging::sendTextF(const __FlashStringHelper* format, ...)
{
	flush(); // Ensure we don't have any pending messages, since we reuse the msg structure
	va_list argptr;
	va_start(argptr, format);
	vsprintf_P(_textMsg.text, (char *)format, argptr);
	va_end(argptr);
	sendMessage(HIVEMSG_TEXT_ID, (uint8_t *)&_textMsg, sizeof(HIVEMSG_TEXT));
}

/**
* Writes raw bytes to the stream
**/
void Messaging::writeBytes(uint8_t * messageBuffer, uint16_t bufferLength)
{
	if (!_isEnabled) {
		return;
	}

	_serialPort->write(messageBuffer, bufferLength);
}

/**
* Reads raw  until the buffer is filled
**/
uint16_t Messaging::readBytes(uint8_t * messageBuffer, uint16_t bufferLength)
{
	if (!_isEnabled) {
		return 0;
	}

	return _serialPort->readBytes(messageBuffer, bufferLength);
}