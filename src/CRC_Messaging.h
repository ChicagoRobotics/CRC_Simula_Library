/***************************************************
Uses: Wrap all communications here. We can implement swap or 
subclass logic to switch between XBEE Zigbee (802.15.4) and XBEE Wifi 
(802.11b/g/n) modules as well as USB communications.

The Messaging module provides for the Remote Command/Communication protocol with 
the central application/server. Binary protocol mode is the only supported
mode, so requires messages from CRC_MessageTypes.h

This file is designed for the Simula project by Chicago Robotics Corp.
http://www.chicagorobotics.net/products

Copyright (c) 2016, Chicago Robotics Corp.
See README.md for license details
****************************************************/

#ifndef _CRC_MESSAGING_h
#define _CRC_MESSAGING_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

#include <HardwareSerial.h>

#include "CRC_MessageTypes.h"

class CRC_Messaging
{
 private:
	 // Communications port
	 boolean _isEnabled;
	 HardwareSerial * _serialPort;
	 uint8_t _chrDataBuffer[260]; // Data Buffer, padded for transport overhead
	 char _bufferPos;
	 uint8_t _calcChecksum;
	 uint8_t _messageChecksum;
	 uint16_t _expectedMsgLength;

	 bool readMessage();
	 void sendMessageImpl(uint8_t messageId, uint8_t * messageBuffer, uint16_t messageLength);
	 static uint8_t calculateChecksum(uint8_t * messageBuffer, uint16_t messageLength);

	 uint8_t _readStatus;
	 uint8_t _messageSequence;
	 HIVEMSG_ACK _ackMsg;
	 HIVEMSG_TEXT _textMsg;
 public:
	 CRC_Messaging();
	 void initZigbeeWifi(HardwareSerial & serialPort, boolean enabled); // Use the Zigbee Wifi Communications Mode
	 void initUSB(HardwareSerial & serialPort, boolean enabled);    // Use the Serial USB Communications Mode

	 bool hasMessage();
	 inline uint8_t* getMessageBuffer() { return _chrDataBuffer; }
	 void clearBuffer();

	 void sendMessage(uint8_t messageId, uint8_t * messageBuffer, uint16_t messageLength);

	 void sendAck(uint8_t messageSequence, uint8_t messageId, uint8_t ackCode, uint8_t reason = 0x00);
	 void sendText(const char* message); 
	 void sendText(const __FlashStringHelper* message);
	 void sendTextF(const char* format, ...);  
	 void sendTextF(const  __FlashStringHelper* format, ...);

	 inline void setEnabled(boolean enabled) { _isEnabled = enabled; }
	 void flush();

	 // Raw Streaming (For tasks such as File Transfer)

	 void writeBytes(uint8_t * messageBuffer, uint16_t bufferLength);
	 uint16_t readBytes(uint8_t * messageBuffer, uint16_t bufferLength);
};


#endif // _CRC_MESSAGING_h
