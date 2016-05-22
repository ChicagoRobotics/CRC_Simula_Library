/***************************************************
Uses: Helper for working with Flash/Program Strings, especially 
where we have to pass them to functions that take RAM char*
Allows reusing/sharing the same temp buffers. 
Buffers are allocated by execution contexts (main for main, thread
for thread). Do not mix the usage, and do not use these buffers
outside of an immediate execute and dispose model.

This file is designed for the Simula project by Chicago Robotics Corp.
http://www.chicagorobotics.net/products

Copyright (c) 2016, Chicago Robotics Corp.
See README.md for license details
****************************************************/

#ifndef _CRC_FLASHHELPER_h
#define _CRC_FLASHHELPER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#define CRC_FHM(string_literal) (CRC_FlashHelper::main(F(string_literal)))

#define CRC_FH_BUFFER_LEN 255
class CRC_FlashHelper
{
private:
	// These are Temp buffers in RAM 
	// Used to transfer the text from Flash

	static char _mainBuffer[CRC_FH_BUFFER_LEN]; // Buffer invoked from Main code

public:
	// Do not invoke from an IRQ or chain in same call
	static char* main(const __FlashStringHelper * flashString);
};


#endif

