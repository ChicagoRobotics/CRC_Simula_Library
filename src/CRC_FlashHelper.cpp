/***************************************************
Uses: Helper for working with Flash/Program Strings, especially
where we have to pass them to functions that take RAM char*
Allows reusing the same buffers.
Buffers are allocated by execution contexts (main for main, thread
for thread). Do not mix the usage, and do not use these buffers
outside of an immediate execute and dispose model.

This file is designed for the Simula project by Chicago Robotics Corp.
http://www.chicagorobotics.net/products

Copyright (c) 2016, Chicago Robotics Corp.
See README.md for license details
****************************************************/

#include "CRC_FlashHelper.h"

char CRC_FlashHelper::_mainBuffer[CRC_FH_BUFFER_LEN];

char* CRC_FlashHelper::main(const __FlashStringHelper * flashString)
{
	strcpy_P(CRC_FlashHelper::_mainBuffer, (const char *) flashString);
	return CRC_FlashHelper::_mainBuffer;
}

